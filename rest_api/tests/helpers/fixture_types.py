from typing import Final, Iterable, List, Type, Union

from pydantic import BaseModel, Field, NonNegativeInt

from rest_api.models import WatchList

TestWatchListReturnType: Final[Type] = Union[WatchList, Iterable[WatchList]]


class UserWithWatchListsParam(BaseModel):
    users_watchlists: List[NonNegativeInt] = Field()


class TestWatchListParam(BaseModel):
    watchlist_count: NonNegativeInt = Field()
