Проект состоит из двух частей:
1. server/volunteerServer — серверная часть на Python, FastAPI.
2. client/volunteerClient — мобильное приложение iOS на SwiftUI.

P.S. Файл .env не включён в архив проекта, так как он содержит локальные настройки, пароли и токены.
Перед запуском серверной части необходимо создать файл .env вручную.

1. ТРЕБОВАНИЯ ДЛЯ ЗАПУСКА

Для серверной части:
- Python 3.11 или выше;
- PostgreSQL;
- pip.

Для клиентской части:
- macOS;
- Xcode;
- iOS Simulator или реальное iOS-устройство.

2. НАСТРОЙКА ФАЙЛА .env

В папке server/volunteerServer необходимо создать файл с названием .env
В этот файл нужно добавить следующие настройки:

DATABASE_URL=postgresql+psycopg://app_user:StrongPass_123!@127.0.0.1:5432/app_db
SECRET_KEY=change-this-secret-key-to-a-long-random-string
DEFAULT_ADMIN_USERNAME=admin
DEFAULT_ADMIN_PASSWORD=Admin12345!

YANDEX_GEOCODER_API_KEY=
YANDEX_GEOCODER_URL=https://geocode-maps.yandex.ru/v1/
YANDEX_GEOCODER_LANG=ru_RU
YANDEX_GEOCODER_TIMEOUT_SECONDS=5
YANDEX_GEOCODER_RESULTS_LIMIT=5

GEOCODE_RATE_LIMIT_PER_MINUTE=60
FORWARD_GEOCODE_CACHE_TTL_SECONDS=86400
REVERSE_GEOCODE_CACHE_TTL_SECONDS=86400
GEOCODE_NEGATIVE_CACHE_TTL_SECONDS=60
REVERSE_GEOCODE_COORDINATE_PRECISION=5

TELEGRAM_BOT_TOKEN=
TELEGRAM_ADMIN_IDS=

SMTP_HOST=smtp.gmail.com
SMTP_PORT=587
SMTP_USERNAME=
SMTP_PASSWORD=
SMTP_FROM_EMAIL=
SMTP_FROM_NAME=Volunteer App

Обязательные параметры для базового запуска:
DATABASE_URL
SECRET_KEY
DEFAULT_ADMIN_USERNAME
DEFAULT_ADMIN_PASSWORD

Параметры YANDEX_GEOCODER_API_KEY, TELEGRAM_BOT_TOKEN и SMTP_* можно оставить пустыми.
В этом случае функции геокодирования, Telegram-администрирования и отправки писем могут быть недоступны.

3. НАСТРОЙКА БАЗЫ ДАННЫХ

Проект использует PostgreSQL.Если используется строка подключения по умолчанию:

postgresql+psycopg://app_user:StrongPass_123!@127.0.0.1:5432/app_db

нужно создать базу данных app_db и пользователя app_user.

4. ЗАПУСК СЕРВЕРНОЙ ЧАСТИ

Перейти в папку серверной части cd server/volunteerServer

Создать виртуальное окружение: python -m venv .venv

Активировать виртуальное окружение:

Для macOS/Linux:

source .venv/bin/activate

Для Windows:

.venv\Scripts\activate

Установить зависимости:

pip install fastapi uvicorn sqlalchemy psycopg[binary] python-dotenv pydantic[email] pwdlib PyJWT python-multipart

Запустить сервер:

uvicorn app.main:app --host 0.0.0.0 --port 8000 --reload

После запуска сервер будет доступен по адресу:

http://127.0.0.1:8000

При первом запуске таблицы базы данных создаются автоматически.
Также автоматически создаётся администратор с логином и паролем из файла .env.

5. ЗАПУСК КЛИЕНТСКОЙ ЧАСТИ

Открыть проект в Xcode: client/volunteerClient/volunteerClient.xcworkspace

Перед запуском проверить адрес сервера в файле: client/volunteerClient/volunteerClient/App/AppConfig.swift

Если приложение запускается в iOS Simulator на том же компьютере, можно указать:

static let baseURLString = "http://127.0.0.1:8000"

Если приложение запускается на реальном iPhone, нужно указать IP-адрес компьютера в локальной сети, например:

static let baseURLString = "http://192.168.0.207:8000"

При этом iPhone и компьютер должны быть подключены к одной Wi-Fi сети, а сервер должен быть запущен с параметром:

--host 0.0.0.0

После этого в Xcode выбрать симулятор или устройство и нажать Run.

6. ПОРЯДОК ЗАПУСКА

1. Установить PostgreSQL.
2. Создать базу данных и пользователя.
3. Создать файл .env в папке server/volunteerServer.
4. Запустить серверную часть командой uvicorn.
5. Проверить адрес http://127.0.0.1:8000/health.
6. Открыть client/volunteerClient/volunteerClient.xcworkspace в Xcode.
7. Проверить адрес сервера в AppConfig.swift.
8. Запустить приложение через Xcode.
