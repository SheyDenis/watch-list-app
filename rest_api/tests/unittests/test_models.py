import datetime
from typing import Set
from uuid import uuid4

import pytest
from django.forms import model_to_dict

from rest_api.models import User, UserDTO, WatchList, WatchListDTO
from watch_list_common.datetime_utils import DatetimeUtils


@pytest.mark.django_db(transaction=True)
class TestModelConversion:

    def test_user_model_to_pydantic(self, test_user: User):
        user_pydantic_model = UserDTO.from_model(test_user)

        assert {
            'id': test_user.id,
            'username': test_user.username,
            'first_name': test_user.first_name,
            'last_name': test_user.last_name,
            'email': test_user.email,
            'is_staff': test_user.is_staff,
            'is_active': test_user.is_active,
            'date_joined': test_user.date_joined,
            'last_login': test_user.last_login,
        } == {
            # Excluded properties.
            'id': user_pydantic_model.id,
            'is_staff': user_pydantic_model.is_staff,
            'is_active': user_pydantic_model.is_active,
            'date_joined': user_pydantic_model.date_joined,
            'last_login': user_pydantic_model.last_login,
            **user_pydantic_model.model_dump(),
        }

    def test_user_pydantic_to_model(self):
        user_pydantic_model = UserDTO(id=1, username='test username', first_name='test first name', last_name='test last name',
                                      email='test_email@example.com', is_staff=True, is_active=True,
                                      date_joined=datetime.datetime(2024, 5, 11, 20, 40,
                                                                    11), last_login=datetime.datetime(2024, 5, 11, 20, 45, 14))
        user_model = UserDTO.to_model(user_pydantic_model)

        pydantic_excluded_fields: Set[str] = {'id', 'is_staff', 'is_active', 'date_joined', 'last_login'}
        assert {
            'id': user_model.id,
            'username': user_model.username,
            'first_name': user_model.first_name,
            'last_name': user_model.last_name,
            'email': user_model.email,
            'is_staff': user_model.is_staff,
            'is_active': user_model.is_active,
            'date_joined': user_model.date_joined,
            'last_login': user_model.last_login,
        } == {
            **{
                p: getattr(user_pydantic_model, p) for p in pydantic_excluded_fields
            },
            **user_pydantic_model.model_dump(),
        }

        assert model_to_dict(user_model, exclude=pydantic_excluded_fields
                             | {'groups', 'password', 'is_superuser', 'user_permissions'}) == user_pydantic_model.model_dump()

    def test_watchlist_model_to_pydantic(self, test_user: User, test_watchlist: WatchList):
        watchlist_pydantic_model = WatchListDTO.from_model(test_watchlist)

        assert {
            'uuid': test_watchlist.uuid,
            'user': test_watchlist.user_id,
            'name': test_watchlist.name,
            'date_created': test_watchlist.date_created,
            'date_modified': test_watchlist.date_modified,
        } == {
            # Excluded properties.
            'user': watchlist_pydantic_model.user,
            **watchlist_pydantic_model.model_dump(),
        }

    def test_watchlist_pydantic_to_model(self, test_user: User):
        watchlist_pydantic_model = WatchListDTO(uuid=uuid4(), user=test_user.id, name='test watchlist name',
                                                date_created=DatetimeUtils.now() - datetime.timedelta(hours=1),
                                                date_modified=DatetimeUtils.now() - datetime.timedelta(minutes=10))
        watchlist_model = WatchListDTO.to_model(watchlist_pydantic_model)

        assert {
            'uuid': watchlist_model.uuid,
            'user': watchlist_model.user_id,
            'name': watchlist_model.name,
            'date_created': watchlist_model.date_created,
            'date_modified': watchlist_model.date_modified,
        } == {
            'user': None,  # Requires manual assignment.
            **watchlist_pydantic_model.model_dump(),
        }
        assert model_to_dict(watchlist_model) == watchlist_pydantic_model.model_dump()
