from typing import Any, Dict, Optional, Set, Type, TypeVar, cast

from django.db import models
from pydantic import BaseModel

T = TypeVar('T', bound=BaseModel)


class BaseConfig:
    """Base config class for pydantic models."""
    validate_assignment = True
    extra = 'forbid'

    # Add 'by_alias' as default when dumping model.
    by_alias_by_default: bool = True

    # Add exclude properties by default if they are None.
    excluded_none_by_default: Set[str] = {'id'}

    # Properties to always be excluded when dumping models.
    exclude_always: Set[str] = {'user', 'password'}


class ConvertibleConfig:
    """Base config class for pydantic models that can be converted to and from Django models."""

    # The Django model to convert to and from.
    ModelType: Type[models.Model]

    # Properties to exclude when converting from django.db.models.Model object.
    from_model_exclude_always: Set[str] = {'_state'}
    # Additional properties to exclude when converting from django.db.models.Model object.
    from_model_exclude: Set[str] = set()
    # Properties to rename.
    from_model_rename: Dict[str, str] = {}

    # Properties to exclude when converting to django.db.models.Model object.
    to_model_exclude_always: Set[str] = {'user'}
    # Additional properties to exclude when converting to django.db.models.Model object.
    to_model_exclude: Set[str] = set()
    # Properties to rename.
    to_model_rename: Dict[str, str] = {}


def model_dump_wrapper(obj: BaseModel, by_alias: bool = True, **kwargs) -> Dict[str, Any]:
    if not issubclass(obj.Config, BaseConfig):
        raise ValueError(f'Object [{type(obj).__name__}] must have a config of type [{BaseConfig.__name__}] has [{obj.Config.__name__}]')
    if obj.Config.by_alias_by_default and 'by_alias' not in kwargs:
        kwargs['by_alias'] = by_alias

    excludes = __generate_excludes(obj, kwargs.get('exclude', None), kwargs.get('include', None))
    if len(excludes) > 0:
        kwargs['exclude'] = excludes
    return cast(BaseModel, super(type(obj), obj)).model_dump(**kwargs)


def model_dump_json_wrapper(obj: BaseModel, by_alias: bool = True, **kwargs) -> str:
    if not issubclass(obj.Config, BaseConfig):
        raise ValueError(f'Object [{type(obj).__name__}] must have a config of type [{BaseConfig.__name__}] has [{obj.Config.__name__}]')

    if obj.Config.by_alias_by_default and 'by_alias' not in kwargs:
        kwargs['by_alias'] = by_alias

    excludes = __generate_excludes(obj, kwargs.get('exclude', None), kwargs.get('include', None))
    if len(excludes) > 0:
        kwargs['exclude'] = excludes

    return cast(BaseModel, super(type(obj), obj)).model_dump_json(**kwargs)


class ConvertibleMixin(BaseModel):

    @classmethod
    def from_model(cls: Type[T], model: models.Model) -> T:
        if not issubclass(cls.Config, ConvertibleConfig):
            raise ValueError(
                f'Object [{cls.__name__}] must have a config of type [{ConvertibleConfig.__name__}] has [{cls.Config.__name__}]')

        return cls(
            **{
                cls.Config.from_model_rename.get(k, k): v
                for k, v in model.__dict__.items()
                if k not in cls.Config.from_model_exclude_always | cls.Config.from_model_exclude
            })

    @classmethod
    def to_model(cls: Type[T], obj: BaseModel):
        if not issubclass(cls.Config, ConvertibleConfig):
            raise ValueError(
                f'Object [{cls.__name__}] must have a config of type [{ConvertibleConfig.__name__}] has [{cls.Config.__name__}]')

        model: models.Model = cls.Config.ModelType()

        for k, v in vars(obj).items():
            if k in cls.Config.to_model_exclude_always | cls.Config.to_model_exclude:
                continue
            setattr(model, cls.Config.to_model_rename.get(k, k), v)

        return model


def __generate_excludes(obj: BaseModel, exclude: Optional[Set[str]], include: Optional[Set[str]]) -> Set[str]:
    excludes: Set[str] = set()
    if exclude:
        excludes = exclude
    if include is None:
        include = set()

    config = cast(BaseConfig, obj.Config)
    if exclude is None:
        for prop in config.excluded_none_by_default:
            if prop in include:
                continue
            if hasattr(obj, prop) and getattr(obj, prop) is None:
                excludes.add(prop)
    for prop in config.exclude_always:
        if prop in include:
            raise RuntimeError(f'Property [{prop}] was included but marked as always excluded')
        if hasattr(obj, prop):
            excludes.add(prop)
    return excludes
