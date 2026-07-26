add_executable(Combat-Tracker-App
    main.cpp
    "../CombatTrackerLib/Utilities/JSONUtils.h"
    

    Entities/Creature/Creature.h
    Entities/Creature/Creature.cpp
    Entities/Creature/CreatureConstants.h
    Entities/Creature/CreatureEnums.h
    Entities/Creature/CreatureTypes.h

    Entities/Encounter/Encounter.h
    Entities/Encounter/Encounter.cpp

    Entities/Monster/Monster.h
    Entities/Monster/Monster.cpp
    Entities/Monster/MonsterConstants.h

    Entities/PlayerCharacter/PlayerCharacter.h
    Entities/PlayerCharacter/PlayerCharacter.cpp
    Entities/PlayerCharacter/PlayerCharacterConstants.h
    Entities/PlayerCharacter/PlayerCharacterEnums.h

    Entities/Turn/Turn.h
    Entities/Turn/Turn.cpp

    Scenes/EncounterCreationScene.h
    Scenes/EncounterCreationScene.cpp
    Scenes/MainMenuScene.h
    Scenes/MainMenuScene.cpp
    Scenes/OverlayScene.h
    Scenes/OverlayScene.cpp
 )

# Link the static library we created earlier to the application.
target_link_libraries(
    Combat-Tracker-App PRIVATE
    Combat-Tracker-Lib
)

# This is needed so that the assets can be accessed from the build directory.
add_custom_command(TARGET Combat-Tracker-App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Combat-Tracker-App>/assets
    COMMENT "Copying assets to the output directory"
)