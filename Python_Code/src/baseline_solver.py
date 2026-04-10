from __future__ import annotations

from functools import lru_cache
from typing import List, Tuple

from src.models import ProblemInstance, Route, Solution


class BaselineExactSolver:
    """
    Exact baseline solver for the 2-vehicle CVRP assignment instance.

    Approach:
    1. Enumerate feasible customer partitions for vehicle 1.
    2. Assign the complement to vehicle 2.
    3. For each partition, solve the optimal depot-start / depot-end route
       using Held-Karp dynamic programming.
    4. Return the best combined solution.

    This is intended as a baseline/benchmark for the current assignment,
    not as the final scalable column-generation implementation.
    """

    def __init__(self, instance: ProblemInstance) -> None:
        if instance.vehicle_count != 2:
            raise ValueError(
                "BaselineExactSolver currently supports exactly 2 vehicles. "
                "Use the future column-generation implementation for the general case."
            )

        self.instance = instance
        self.depot = instance.depot
        self.customer_nodes = instance.customer_nodes
        self.num_customers = len(self.customer_nodes)
        self.full_mask = (1 << self.num_customers) - 1

        self.customer_demands = [instance.demands[node] for node in self.customer_nodes]
        self.depot_distances = [instance.distance_matrix[self.depot][node] for node in self.customer_nodes]
        self.customer_distances = [
            [instance.distance_matrix[src][dst] for dst in self.customer_nodes]
            for src in self.customer_nodes
        ]

        self.subset_demand = self._build_subset_demands()

    def solve(self) -> Solution:
        total_demand = self.subset_demand[self.full_mask]
        capacity = self.instance.capacity_per_vehicle

        min_required_for_vehicle_1 = max(0, total_demand - capacity)
        max_allowed_for_vehicle_1 = capacity

        best_total_distance = float("inf")
        best_mask: int | None = None
        partitions_evaluated = 0

        for mask in range(1 << self.num_customers):
            demand_vehicle_1 = self.subset_demand[mask]
            if demand_vehicle_1 < min_required_for_vehicle_1 or demand_vehicle_1 > max_allowed_for_vehicle_1:
                continue

            complement = self.full_mask ^ mask

            # Avoid evaluating both a subset and its complement.
            if mask > complement:
                continue

            if not self._is_feasible_partition(mask):
                continue

            partitions_evaluated += 1
            cost_1, _ = self._best_route_cost(mask)
            cost_2, _ = self._best_route_cost(complement)
            total_cost = cost_1 + cost_2

            if total_cost < best_total_distance:
                best_total_distance = total_cost
                best_mask = mask

        if best_mask is None:
            raise RuntimeError("No feasible partition found for the instance.")

        route_1 = self._reconstruct_route(best_mask, vehicle_id=1)
        route_2 = self._reconstruct_route(self.full_mask ^ best_mask, vehicle_id=2)

        routes = sorted([route_1, route_2], key=lambda route: (route.distance, route.nodes))

        return Solution(
            routes=routes,
            total_distance=int(best_total_distance),
            method="exact_baseline_partition_plus_dp",
            metadata={
                "partitions_evaluated": partitions_evaluated,
                "best_partition_mask": best_mask,
            },
        )

    def _build_subset_demands(self) -> List[int]:
        subset_demand = [0] * (1 << self.num_customers)
        for mask in range(1, 1 << self.num_customers):
            lsb = mask & -mask
            bit = lsb.bit_length() - 1
            subset_demand[mask] = subset_demand[mask ^ lsb] + self.customer_demands[bit]
        return subset_demand

    def _is_feasible_partition(self, mask: int) -> bool:
        demand_1 = self.subset_demand[mask]
        demand_2 = self.subset_demand[self.full_mask ^ mask]
        capacity = self.instance.capacity_per_vehicle
        return demand_1 <= capacity and demand_2 <= capacity

    @lru_cache(maxsize=None)
    def _tsp_path_to_last(self, mask: int, last_idx: int) -> Tuple[int, int]:
        prev_mask = mask ^ (1 << last_idx)
        if prev_mask == 0:
            return self.depot_distances[last_idx], -1

        best_cost = float("inf")
        best_prev = -1

        remaining = prev_mask
        while remaining:
            lsb = remaining & -remaining
            prev_idx = lsb.bit_length() - 1
            prev_cost, _ = self._tsp_path_to_last(prev_mask, prev_idx)
            candidate = prev_cost + self.customer_distances[prev_idx][last_idx]
            if candidate < best_cost:
                best_cost = candidate
                best_prev = prev_idx
            remaining ^= lsb

        return int(best_cost), best_prev

    @lru_cache(maxsize=None)
    def _best_route_cost(self, mask: int) -> Tuple[int, int]:
        if mask == 0:
            return 0, -1

        best_cost = float("inf")
        best_last = -1
        remaining = mask

        while remaining:
            lsb = remaining & -remaining
            last_idx = lsb.bit_length() - 1
            path_cost, _ = self._tsp_path_to_last(mask, last_idx)
            candidate = path_cost + self.depot_distances[last_idx]
            if candidate < best_cost:
                best_cost = candidate
                best_last = last_idx
            remaining ^= lsb

        return int(best_cost), best_last

    def _reconstruct_route(self, mask: int, vehicle_id: int) -> Route:
        route_cost, last_idx = self._best_route_cost(mask)
        if mask == 0:
            return Route(vehicle_id=vehicle_id, nodes=[self.depot, self.depot], load=0, distance=0)

        reverse_route = [self.customer_nodes[last_idx]]
        current_mask = mask
        current_last = last_idx

        while True:
            _, predecessor = self._tsp_path_to_last(current_mask, current_last)
            if predecessor == -1:
                break
            current_mask ^= 1 << current_last
            current_last = predecessor
            reverse_route.append(self.customer_nodes[current_last])

        customer_sequence = list(reversed(reverse_route))
        nodes = [self.depot, *customer_sequence, self.depot]
        load = sum(self.instance.demands[node] for node in customer_sequence)

        return Route(vehicle_id=vehicle_id, nodes=nodes, load=load, distance=route_cost)
