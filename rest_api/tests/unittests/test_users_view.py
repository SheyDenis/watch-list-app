from http import HTTPStatus
from typing import Final, List

import pytest
from django.test import Client

from rest_api.models import User, WatchList


@pytest.mark.django_db(transaction=True)
class TestUsersView:

    @pytest.fixture(name='setup_test_users', scope='function')
    def __setup_test_users(self, api_client) -> None:
        self.test_user_1 = User.objects.create_user(
            username='test_user_1',
            email='test_user_1@example.com',
            password='test_password_1',
            first_name='test_first_name_1',
            last_name='test_last_name_1',
        )
        self.test_user_2 = User.objects.create_user(
            username='test_user_2',
            email='test_user_2@example.com',
            password='test_password_2',
            first_name='test_first_name_2',
            last_name='test_last_name_2',
        )
        self.test_user_3 = User.objects.create_user(
            username='test_user_3',
            email='test_user_3@example.com',
            password='test_password_3',
            first_name='test_first_name_3',
            last_name='test_last_name_3',
        )

        res = api_client.post('/session/', data={
            'username': self.test_user_2.username,
            'password': 'test_password_2',
        })
        assert res.status_code == HTTPStatus.OK

        yield
        res = api_client.delete('/session/')
        assert res.status_code == HTTPStatus.OK

    @pytest.mark.usefixtures('setup_test_users')
    def test_get_success(self, api_client: Client):
        res = api_client.get('/api/users/')
        assert res.status_code == HTTPStatus.OK
        self.test_user_2.refresh_from_db()

        assert {
            k: v for k, v in res.json().items() if k not in ('last_login',)
        } == {
            'username': 'test_user_2',
            'email': 'test_user_2@example.com',
            'first_name': 'test_first_name_2',
            'last_name': 'test_last_name_2',
            'watch_lists': {},
        }

    @pytest.mark.usefixtures('setup_test_users')
    def test_get_watch_lists(self, api_client: Client):
        num_wl_per_user: Final[int] = 3
        _: List[WatchList] = [WatchList.objects.create(user=self.test_user_1, name=f'watch_list_{idx}') for idx in range(num_wl_per_user)]
        user_watch_lists_2: List[WatchList] = [
            WatchList.objects.create(user=self.test_user_2, name=f'watch_list_{idx}') for idx in range(num_wl_per_user)
        ]
        _: List[WatchList] = [WatchList.objects.create(user=self.test_user_3, name=f'watch_list_{idx}') for idx in range(num_wl_per_user)]

        res = api_client.get('/api/users/')
        assert res.status_code == HTTPStatus.OK
        self.test_user_2.refresh_from_db()

        assert {
            k: v for k, v in res.json().items() if k not in ('last_login',)
        } == {
            'username': 'test_user_2',
            'email': 'test_user_2@example.com',
            'first_name': 'test_first_name_2',
            'last_name': 'test_last_name_2',
            'watch_lists': {
                str(wl.uuid): wl.name for wl in user_watch_lists_2
            },
        }
