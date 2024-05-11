from typing import Optional

from django.contrib.auth.models import AbstractUser
from pydantic import BaseModel, EmailStr, Field

from watch_list_common.pydantic_utils import BaseConfig, model_dump_json_wrapper, model_dump_wrapper


class User(AbstractUser):
    """Application user."""
    pass


class UserDTO(BaseModel):
    id: int = Field(exclude=True)
    username: str = Field()
    first_name: Optional[str] = Field()
    last_name: Optional[str] = Field()
    email: Optional[EmailStr] = Field()
    is_staff: bool = Field(exclude=True)
    is_active: bool = Field(exclude=True)

    class Config(BaseConfig):
        pass

    model_dump = model_dump_wrapper
    model_dump_json = model_dump_json_wrapper
