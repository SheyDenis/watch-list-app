import pytest
from django.test.runner import DiscoverRunner


class PyTestRunner(DiscoverRunner):

    def run_tests(self, test_labels, extra_tests=None, **kwargs):
        pytest_args = []
        if len(test_labels) > 0:
            pytest_args.extend(['-k', ' or '.join(test_labels)])
        pytest.main(pytest_args)
