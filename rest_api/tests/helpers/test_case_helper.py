from typing import List

from rest_api.models import WatchList
from rest_api.models.user import User


class TestCaseHelper:

    def __new__(cls, *args, **kwargs):
        raise RuntimeError(f'Do not create instances of [{cls.__name__}] class')

    @staticmethod
    def add_watchlists_to_user(user: User, count: int) -> List[WatchList]:
        if count < 0:
            raise ValueError(f'count must be >=0 [{count=}]')

        watchlists: List[WatchList] = [WatchList.objects.create(user=user, name=f'test_watchlist_{i+1}') for i in range(count)]

        return watchlists
