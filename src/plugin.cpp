/**
 * @file plugin.cpp
 * @brief The main file of the plugin
 */
#include "version.h"
#include <llapi/LoggerAPI.h>
#include <string>

#include <llapi/MC/CommandOrigin.hpp>
#include <llapi/MC/CommandOutput.hpp>
#include <llapi/MC/ItemStack.hpp>
#include <llapi/MC/Level.hpp>
#include <llapi/MC/Player.hpp>
#include <llapi/MC/Types.hpp>

#include <llapi/DynamicCommandAPI.h>
#include <llapi/EventAPI.h>
#include <llapi/GlobalServiceAPI.h>



// We recommend using the global logger.
extern Logger logger;

//global_variable
std::string latest_player_xuid;

/**
 * @brief The entrypoint of the plugin. DO NOT remove or rename this function.
 *        
 */
void PluginInit()
{
    // Your code here
    const string strTitleText = "\
\
        ___              __  _      _____ ____ _____\
        /   |  __________/ /_(_)____ / ___// __ \__  /\
        / /| | / ___ / ___ / __/ / ___ / __ \/ / / //_ < \
        / ___ |/ /  / /__/ /_/ / /__/ /_/ / /_/ /__/ /\
        /_/  |_ / _ / \___ / \__ / _ / \___ / \____ / \____ / ____ /\
\
\
    __QQ:621206218__";

    Logger logger(PLUGIN_NAME);
    logger.info(strTitleText);

    //监听玩家的加入情况
    Event::PlayerJoinEvent::subscribe([](const Event::PlayerJoinEvent& event) {
        // Give the item to the player
        auto* item = ItemStack::create("minecraft:emerald", /* count = */ 1);
        event.mPlayer->giveItem(item);
        delete item;

        // Show banner on every player's screen
        auto all_player_list = Level::getAllPlayers();

        //可以在此进行黑名单的处理
        for (auto* player : all_player_list) {
            player->sendTitlePacket(
                event.mPlayer->getRealName() + "joined the game",
                TitleType::SetTitle,
                /* FadeInDuration = */ 1,
                /* RemainDuration = */ 3,
                /* FadeOutDuration = */ 1);
        }

        return true;
    });

    DynamicCommand::setup(
        /* name = */ "latest",
        /* description = */ "Get latest player.",
        /* enums = */{},
        /* params = */{},
        /* overloads = */{
          {},
        },
        /* callback = */ [](
            DynamicCommand const& command,
            CommandOrigin const& origin,
            CommandOutput& output,
            std::unordered_map<std::string, DynamicCommand::Result>& results) {
                output.success(
                    std::string("The latest player's name is ") +
                    Global<Level>->getPlayer(latest_player_xuid)->getRealName()
                );
        }
    );

    return;
}
