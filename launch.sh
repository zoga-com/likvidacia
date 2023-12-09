#!/bin/bash

if [[ ! -v LIQUIDATOR_TELEGRAM_TOKEN ]]; then
    echo "Переменная окружения LIQUIDATOR_TELEGRAM_TOKEN не указана."
elif [[ -z "$LIQUIDATOR_TELEGRAM_TOKEN" ]]; then
    echo "Переменная окружения LIQUIDATOR_TELEGRAM_TOKEN не указана."
else
    echo "Запуск бота с токеном: $LIQUIDATOR_TELEGRAM_TOKEN"
    cd build/
    make
    ./testapp
fi
