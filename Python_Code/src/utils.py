from __future__ import annotations

from typing import Iterable, List



def route_distance(route: List[int], distance_matrix: List[List[int]]) -> int:
    return sum(distance_matrix[route[i]][route[i + 1]] for i in range(len(route) - 1))



def route_load(route: List[int], demands: List[int], depot: int = 0) -> int:
    return sum(demands[node] for node in route if node != depot)



def format_route(route: Iterable[int]) -> str:
    return " -> ".join(map(str, route))
