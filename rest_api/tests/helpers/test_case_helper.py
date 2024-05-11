import json
from typing import List, Union

from django.http import HttpResponse
from pydantic import BaseModel

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

    @staticmethod
    def assert_response_json_eq(res: HttpResponse, expected: Union[BaseModel, dict, list]):
        """Assert the response JSON is equal to the expected JSON."""

        if not isinstance(res, HttpResponse):
            raise RuntimeError(f'Unsupported response type [{type(res).__name__}]')

        expected_res = expected
        if isinstance(expected, BaseModel):
            # Dump and load to force encoding UUID/Datetime/etc objects.
            expected_res = json.loads(expected.model_dump_json())

        assert res.json() == expected_res
