import datetime
from http import HTTPStatus
from typing import Any, Callable, Dict, Iterable, List, Optional, Tuple
from uuid import UUID, uuid4

import pytest
from django.http import HttpResponse
from django.test import Client

from rest_api.models import User, WatchList, WatchListDTO
from rest_api.tests.helpers import DatetimeEq, TestCaseHelper, TestWatchListParam, TestWatchListReturnType, UserWithWatchListsParam
from watch_list_common.datetime_utils import DatetimeUtils
from watch_list_common.pydantic_utils import RequestErrorModel


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

    @pytest.mark.parametrize(('path', 'method', 'method_kwargs', 'is_required'), (
        pytest.param('/api/watch_lists/', Client.delete, None, True, id='DELETE'),
        pytest.param(f'/api/watch_lists/{str(uuid4())}/', Client.post, {
            'data': WatchListDTO(name='Some name').model_dump()
        }, False, id='POST'),
        pytest.param(f'/api/watch_lists/', Client.patch, {
            'data': WatchListDTO(name='Some name').model_dump()
        }, True, id='PATCH'),
    ))
    def test_bad_request_resource_id(self, path: str, method: Callable[..., HttpResponse], method_kwargs: Optional[Dict[str, Any]],
                                     is_required: bool, api_client: Client):
        if method_kwargs is None:
            method_kwargs = {}
        res = method(api_client, path, **method_kwargs)
        assert res.status_code == HTTPStatus.BAD_REQUEST

        if is_required:
            TestCaseHelper.assert_response_json_eq(res, RequestErrorModel(detail='method requires resource id'))
        else:
            TestCaseHelper.assert_response_json_eq(res, RequestErrorModel(detail='method not supported with resource id'))

    def test_get_success(self, test_watchlist: WatchList, api_client: Client):
        res = api_client.get(f'/api/watch_lists/{test_watchlist.uuid}/')
        assert res.status_code == HTTPStatus.OK

        res_json = res.json()
        _ = UUID(hex=res_json['uuid'])  # Verify response uuid is valid.
        TestCaseHelper.assert_response_json_eq(
            res, {
                'uuid': str(test_watchlist.uuid),
                'name': 'test_watchlist_1',
                'date_created': DatetimeEq(test_watchlist.date_created, datetime.timedelta(seconds=1)),
                'date_modified': DatetimeEq(test_watchlist.date_modified, datetime.timedelta(seconds=1)),
            })

    @pytest.mark.usefixtures('test_watchlist')
    def test_get_no_resource(self, api_client: Client):
        res = api_client.get(f'/api/watch_lists/{str(uuid4())}/')
        assert res.status_code == HTTPStatus.NOT_FOUND

    @pytest.mark.parametrize(('test_watchlist', 'user_with_watchlists'), (
        pytest.param(TestWatchListParam(watchlist_count=0), UserWithWatchListsParam(users_watchlists=[3]), id='No user resource'),
        pytest.param(TestWatchListParam(watchlist_count=1), UserWithWatchListsParam(users_watchlists=[3]), id='Success'),
        pytest.param(TestWatchListParam(watchlist_count=4), UserWithWatchListsParam(users_watchlists=[3]), id='Success'),
    ), indirect=True)
    def test_list(self, test_watchlist: TestWatchListReturnType, user_with_watchlists: Iterable[User], test_user: User, api_client: Client):
        res = api_client.get('/api/watch_lists/')
        assert res.status_code == HTTPStatus.OK

        if isinstance(test_watchlist, WatchList):
            test_watchlist = [test_watchlist]
        res_json = res.json()
        assert len(res_json) == len(test_watchlist)

        TestCaseHelper.assert_response_json_eq(res, [{
            'uuid': str(wl.uuid),
            'name': wl.name,
            'date_created': DatetimeUtils.format_datetime(wl.date_created),
            'date_modified': DatetimeUtils.format_datetime(wl.date_modified),
        } for wl in test_watchlist])
        for wl in test_watchlist:
            assert wl.date_created is not None, wl
            assert wl.date_modified is not None, wl

    @pytest.mark.parametrize(('test_watchlist', 'user_with_watchlists', 'create_request', 'expected_status_code'),
                             (
                                 pytest.param(
                                     TestWatchListParam(watchlist_count=0), UserWithWatchListsParam(users_watchlists=[3]),
                                     WatchListDTO(name='unique name'), HTTPStatus.CREATED, id='Success'),
                                 pytest.param(
                                     TestWatchListParam(watchlist_count=0), UserWithWatchListsParam(users_watchlists=[3]),
                                     WatchListDTO(name='test_watchlist_1'), HTTPStatus.CREATED, id='Success existing name of another user'),
                                 pytest.param(
                                     TestWatchListParam(watchlist_count=3), UserWithWatchListsParam(users_watchlists=[3]),
                                     WatchListDTO(name='test_watchlist_1'), HTTPStatus.BAD_REQUEST, id='Fail existing name'),
                             ), indirect=['test_watchlist', 'user_with_watchlists'])
    def test_create(self, test_watchlist: TestWatchListReturnType, user_with_watchlists: Iterable[User], create_request: WatchListDTO,
                    expected_status_code: HTTPStatus, test_user: User, api_client: Client):
        res = api_client.post('/api/watch_lists/', data=create_request.model_dump(), content_type='application/json')
        assert res.status_code == expected_status_code
        assert res.status_code in (HTTPStatus.BAD_REQUEST, HTTPStatus.CREATED)

        resource_uuid: UUID = WatchList.objects.get(user=test_user, name=create_request.name).uuid

        if res.status_code == HTTPStatus.CREATED:
            TestCaseHelper.assert_response_json_eq(res, WatchListDTO(uuid=resource_uuid, **create_request.model_dump()),
                                                   excluded={'date_created', 'date_modified'})
            assert {
                k: res.json()[k] for k in ('date_created', 'date_modified')
            } == {
                'date_created': DatetimeEq(DatetimeUtils.now(), datetime.timedelta(seconds=1)),
                'date_modified': DatetimeEq(DatetimeUtils.now(), datetime.timedelta(seconds=1)),
            }
        else:
            TestCaseHelper.assert_response_json_eq(res, RequestErrorModel(detail='resource with this name already exists'))

    @pytest.mark.parametrize(('test_body', 'expected_status_code', 'expected_response'), (
        pytest.param('{"name":"some name"} }', HTTPStatus.BAD_REQUEST, None, id='Fail create with invalid json (bad json data)'),
        pytest.param('{}', HTTPStatus.BAD_REQUEST, None, id='Fail create with invalid model (missing properties)'),
        pytest.param('{"name":"some name", "unexpected": "something unexpected"}', HTTPStatus.BAD_REQUEST, None,
                     id='Fail create with invalid model (extra properties)'),
        pytest.param(f'{{"name":"some name", "uuid": "{str(uuid4())}"}}', HTTPStatus.BAD_REQUEST,
                     RequestErrorModel(detail='request data must not contain resource id'), id='Fail create with resource_id in model'),
        pytest.param('{"name":123}', HTTPStatus.BAD_REQUEST, None, id='Fail create with invalid model (wrong properties)'),
    ))
    def test_create_bad_request(self, test_body: str, expected_status_code: HTTPStatus, expected_response: Optional[RequestErrorModel],
                                api_client: Client):
        res = api_client.post('/api/watch_lists/', data=test_body, content_type='application/json')
        assert res.status_code == expected_status_code

        if expected_response is None:
            assert len(res.content) == 0, res.content  # Require no body in the response.
        else:
            TestCaseHelper.assert_response_json_eq(res, expected_response)
