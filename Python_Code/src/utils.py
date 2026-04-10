from __future__ import annotations

from typing import Iterable


def format_route(route: Iterable[int]) -> str:
    return " -> ".join(map(str, route))
