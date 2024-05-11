ISORT = isort
YAPF = yapf
CONFIGS_PATH = .

isort:
	${ISORT} --settings-file ${CONFIGS_PATH}/isort.cfg --src rest_api/ --src tools/ --src watch_list_app/ --src watch_list_common/ .

format:
	${YAPF} --in-place --style ${CONFIGS_PATH}/yapf_style --recursive rest_api/ tools/ watch_list_app/ watch_list_common/

all: isort format
