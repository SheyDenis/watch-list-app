from http import HTTPStatus
from typing import Iterable, List

import pytest
from django.test import Client

from rest_api.models import WatchList
from rest_api.models.user import User
from rest_api.tests.helpers import TestCaseHelper, TestWatchListParam, TestWatchListReturnType, UserWithWatchListsParam


@pytest.fixture(name='api_client', scope='function')
def __api_client() -> Client:
    yield Client()


@pytest.fixture(name='test_user', scope='function')
def __setup_test_user(api_client: Client) -> User:
    test_user: User = User.objects.create_user(
        username='test_user',
        email='test_user@example.com',
        password='test_password',
    )

    res = api_client.post('/session/', data={
        'username': test_user.username,
        'password': 'test_password',
    })
    assert res.status_code == HTTPStatus.OK

    yield test_user

    res = api_client.delete('/session/')
    assert res.status_code == HTTPStatus.OK


@pytest.fixture(name='test_watchlist', scope='function')
def __test_watchlist(test_user: User, request: pytest.FixtureRequest) -> TestWatchListReturnType:
    watchlist_count_param: TestWatchListParam = getattr(request, 'param', TestWatchListParam(watchlist_count=1))

    test_watchlists: List[WatchList] = [
        WatchList.objects.create(
            user=test_user,
            name=f'test_watchlist_{i+1}',
        ) for i in range(watchlist_count_param.watchlist_count)
    ]
    if len(test_watchlists) == 1:
        yield test_watchlists[0]
    else:
        yield test_watchlists


@pytest.fixture(name='test_user2', scope='function')
def __test_user2(api_client: Client) -> User:
    test_user: User = User.objects.create_user(
        username='test_user2',
        email='test_user2@example.com',
        password='test_password',
    )

    yield test_user


@pytest.fixture(name='user_with_watchlists', scope='function')
def __user_with_watchlists(request: pytest.FixtureRequest) -> Iterable[User]:
    param: UserWithWatchListsParam = request.param

    users: List[User] = []
    for i, count in enumerate(param.users_watchlists):
        user = User.objects.create(username=f'test_user_{i+1}')
        TestCaseHelper.add_watchlists_to_user(user=user, count=count)

        users.append(user)

    yield users
