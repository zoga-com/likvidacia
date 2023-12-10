#include <filesystem>
#include <iostream>
#include <string>
#include <tgbot/TgException.h>
#include <tgbot/tgbot.h>
#include <tgbot/types/InputFile.h>
#include "main.h"

void launch_bot() {
    TgBot::Bot bot(getenv("LIQUIDATOR_TELEGRAM_TOKEN"));

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        int _randomFile = random_int(1, 9999999);
        try {
            if (message->photo.empty()) {
                std::cout<<"empty photos"<<std::endl;
                return;
            }

            int _max_size = 0;
            int _max_size_index = 0;
            for(int _i = 0; _i < message->photo.size(); _i++) {
                if(message->photo[_i]->width > _max_size) {
                    _max_size = message->photo[_i]->width;
                    _max_size_index = _i;
                }
            }

            TgBot::File::Ptr fl = bot.getApi().getFile(message->photo[_max_size_index]->fileId);
            create_png_from_binary(bot.getApi().downloadFile(fl->filePath), std::to_string(_randomFile) + ".png");
            process_image(std::to_string(_randomFile) + ".png", "done_" + std::to_string(_randomFile) + ".png");
            if(check_photo_allowed("done_" + std::to_string(_randomFile) + ".png")) {
                bot.getApi().sendPhoto(message->chat->id, TgBot::InputFile::fromFile("done_" + std::to_string(_randomFile) + ".png", "png"), "", message->messageId);
            } else {
                bot.getApi().sendMessage(message->chat->id, "Картинка слишком большая, поэтому её не удалось обработать.", false, message->messageId);
            }
            remove_trash_file(std::to_string(_randomFile) + ".png");
        } catch(TgBot::TgException& err) {
            std::cout<<err.what()<<std::endl;
            remove_trash_file(std::to_string(_randomFile) + ".png");
            bot.getApi().sendMessage(message->chat->id, "При обработке произошла ошибка, попробуйте снова.", false, message->messageId);
        }
    });

    try {
        std::cout<<"Bot username: " + bot.getApi().getMe()->username<<std::endl;
        bot.getApi().deleteWebhook();
        TgBot::TgLongPoll longPoll(bot);
        std::cout<<"Polling started"<<std::endl;
        while (true) {
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
}