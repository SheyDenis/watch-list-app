from http import HTTPStatus
from typing import Iterable, List, Tuple
from uuid import UUID, uuid4

import pytest
from django.test import Client

from rest_api.models import User, WatchList
from rest_api.tests.helpers import TestCaseHelper, TestWatchListParam, TestWatchListReturnType, UserWithWatchListsParam


@pytest.mark.usefixtures('test_user')
@pytest.mark.django_db(transaction=True)
class TestWatchListsView:

    @pytest.fixture(name='test_user2', scope='function')
    def __test_user2(self, api_client: Client, test_user2: User) -> Tuple[User, Iterable[WatchList]]:
        test_watchlists: List[WatchList] = TestCaseHelper.add_watchlists_to_user(user=test_user2, count=3)

        yield test_user2, test_watchlists

    def test_delete_success(self, test_watchlist: WatchList, api_client: Client):
        assert len(list(WatchList.objects.filter(pk=test_watchlist.uuid))) == 1

        res = api_client.delete(f'/api/watch_lists/{test_watchlist.uuid}/')
        assert res.status_code == HTTPStatus.OK

        assert len(list(WatchList.objects.filter(pk=test_watchlist.uuid))) == 0

    def test_delete_no_resource(self, test_watchlist: WatchList, api_client: Client):
        db_items = len(list(WatchList.objects.all()))

        res = api_client.delete(f'/api/watch_lists/{str(uuid4())}/')
        assert res.status_code == HTTPStatus.NOT_FOUND

        assert db_items == len(list(WatchList.objects.all()))

    def test_delete_no_user_resource(self, test_watchlist: WatchList, api_client: Client, test_user2: Tuple[User, Iterable[WatchList]]):
        db_items = len(list(WatchList.objects.all()))
        assert db_items == 4

        for user2_watchlist in test_user2[1]:
            res = api_client.delete(f'/api/watch_lists/{str(user2_watchlist.uuid)}/')
            assert res.status_code == HTTPStatus.NOT_FOUND

        assert db_items == len(list(WatchList.objects.all()))

    def test_get_success(self, test_watchlist: WatchList, api_client: Client):
        res = api_client.get(f'/api/watch_lists/{test_watchlist.uuid}/')
        assert res.status_code == HTTPStatus.OK

        res_json = res.json()
        _ = UUID(hex=res_json['uuid'])
        assert res_json == {
            'uuid': str(test_watchlist.uuid),
            'name': 'test_watchlist_1',
        }

    @pytest.mark.usefixtures('test_watchlist')
    def test_get_no_resource(self, api_client: Client):
        res = api_client.get(f'/api/watch_lists/{str(uuid4())}/')
        assert res.status_code == HTTPStatus.NOT_FOUND

    @pytest.mark.parametrize(
        ('test_watchlist', 'user_with_watchlists'),
        (
            pytest.param(TestWatchListParam(watchlist_count=0), UserWithWatchListsParam(users_watchlists=[3]), id='No user resource'),
            pytest.param(TestWatchListParam(watchlist_count=1), UserWithWatchListsParam(users_watchlists=[3]), id='Success'),
            pytest.param(TestWatchListParam(watchlist_count=4), UserWithWatchListsParam(users_watchlists=[3]), id='Success'),
        ),
        indirect=True,
    )
    def test_list(self, test_watchlist: TestWatchListReturnType, test_user: User, api_client: Client, user_with_watchlists: Iterable[User]):
        res = api_client.get('/api/watch_lists/')
        assert res.status_code == HTTPStatus.OK

        if isinstance(test_watchlist, WatchList):
            test_watchlist = [test_watchlist]
        res_json = res.json()
        assert len(res_json) == len(test_watchlist)

        assert res_json == [{
            'uuid': str(wl.uuid),
            'name': wl.name,
        } for wl in test_watchlist]

    # TODO - Create watchlist
    # TODO - Create watchlist with existing name
    # TODO - Create watchlist with existing name of another user (success)
    # @pytest.mark.parametrize(('test_watchlist', 'user_with_watchlists', 'create_request', 'expected_status_code'),
    #                          (
    #                              pytest.param(TestWatchListParam(watchlist_count=0), UserWithWatchListsParam(users_watchlists=[3]),
    #                                           WatchListDTO(name='unique name'),
    #                                           HTTPStatus.CREATED,
    #                                           id='Success'),
    #                              # pytest.param(TestWatchListParam(watchlist_count=1), UserWithWatchListsParam(users_watchlists=[3]),
    #                              #              id='Fail existing name'),
    #                              # pytest.param(TestWatchListParam(watchlist_count=4), UserWithWatchListsParam(users_watchlists=[3]),
    #                              #              id='Success existing name of another user'),
    #                          ),
    #                          indirect=['test_watchlist', 'user_with_watchlists'],
    #                          )
    # def test_create(self, test_watchlist: TestWatchListReturnType, test_user: User, api_client: Client,
    #                       user_with_watchlists: Iterable[User], create_request: WatchListDTO, expected_status_code: HTTPStatus):
    #     res = api_client.post(f'/api/watch_lists/{str(uuid4())}/', data=create_request.dict(exclude={'id', 'user'}))
    #     assert res.status_code == expected_status_code
    #
    #     if res.status_code==HTTPStatus.CREATED:
    #         assert WatchListDTO.parse_obj(res).dict() ==WatchListDTO(user=test_user.id, **create_request.dict()).dict()

    # TODO - Modify watchlist
    # TODO - Modify watchlist that doesn't exists
    # TODO - Modify watchlist to duplicate name
