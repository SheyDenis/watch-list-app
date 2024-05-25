import json
from typing import List, Optional, Set, Union

from django.http import HttpResponse
from pydantic import BaseModel

from rest_api.models import WatchList
from rest_api.models.user import User
from watch_list_common.datetime_utils import DatetimeUtils


class TestCaseHelper:

    def __new__(cls, *args, **kwargs):
        raise RuntimeError(f'Do not create instances of [{cls.__name__}] class')

    @staticmethod
    def add_watchlists_to_user(user: User, count: int) -> List[WatchList]:
        if count < 0:
            raise ValueError(f'count must be >=0 [{count=}]')

        watchlists: List[WatchList] = [
            WatchList.objects.create(user=user, name=f'test_watchlist_{i+1}', date_created=DatetimeUtils.now(),
                                     date_modified=DatetimeUtils.now()) for i in range(count)
        ]

        return watchlists

    @staticmethod
    def assert_response_json_eq(res: HttpResponse, expected: Union[BaseModel, dict, list], *, excluded: Optional[Set[str]] = None):
        """Assert the response JSON is equal to the expected JSON."""

        if not isinstance(res, HttpResponse):
            raise RuntimeError(f'Unsupported response type [{type(res).__name__}]')

        expected_res = expected
        if isinstance(expected, BaseModel):
            # Dump and load to force encoding UUID/Datetime/etc objects.
            expected_res = json.loads(expected.model_dump_json())

        if excluded is None:
            assert res.json() == expected_res
        else:
            assert {
                k: v for k, v in res.json().items() if k not in excluded
            } == {
                k: v for k, v in expected_res.items() if k not in excluded
            }
