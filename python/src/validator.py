from __future__ import annotations

from dataclasses import dataclass

from src.models import ProblemInstance, Solution
from src.utils import route_distance, route_load


@dataclass(frozen=True)
class ValidationSummary:
    all_customers_visited_once: bool
    routes_start_end_at_depot: bool
    capacities_respected: bool
    reported_distances_match: bool
    vehicle_count_matches: bool

    @property
    def is_valid(self) -> bool:
        return (
            self.all_customers_visited_once
            and self.routes_start_end_at_depot
            and self.capacities_respected
            and self.reported_distances_match
            and self.vehicle_count_matches
        )



def validate_solution(instance: ProblemInstance, solution: Solution) -> ValidationSummary:
    seen_customers: list[int] = []
    depot_ok = True
    capacity_ok = True
    distance_ok = True

    for route in solution.routes:
        if route.nodes[0] != instance.depot or route.nodes[-1] != instance.depot:
            depot_ok = False

        customers = [node for node in route.nodes if node != instance.depot]
        seen_customers.extend(customers)

        recomputed_load = route_load(route.nodes, instance.demands, depot=instance.depot)
        if recomputed_load != route.load or recomputed_load > instance.capacity_per_vehicle:
            capacity_ok = False

        recomputed_distance = route_distance(route.nodes, instance.distance_matrix)
        if recomputed_distance != route.distance:
            distance_ok = False

    expected_customers = sorted(instance.customer_nodes)
    all_customers_once = sorted(seen_customers) == expected_customers and len(seen_customers) == len(set(seen_customers))
    total_distance_ok = sum(route.distance for route in solution.routes) == solution.total_distance
    vehicle_count_matches = len(solution.routes) == instance.vehicle_count

    return ValidationSummary(
        all_customers_visited_once=all_customers_once,
        routes_start_end_at_depot=depot_ok,
        capacities_respected=capacity_ok,
        reported_distances_match=distance_ok and total_distance_ok,
        vehicle_count_matches=vehicle_count_matches,
    )
