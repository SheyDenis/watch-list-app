from pydantic import BaseModel, Field, field_validator

from watch_list_common.pydantic_utils import BaseConfig


class RequestErrorModel(BaseModel):
    """Generic error model for request errors."""
    detail: str = Field()

    class Config(BaseConfig):
        pass

    @field_validator('detail', mode='after')
    def detail_field_validator(cls, value: str) -> str:
        if not value.endswith('.'):
            value += '.'

        if value[0].isupper():
            raise ValueError('Error details must start with a lower-case letter')
        return value
