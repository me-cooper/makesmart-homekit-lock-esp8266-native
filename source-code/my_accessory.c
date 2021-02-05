#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
  // Diese Funktion wird aufgreufen, wenn das Accessorie identifiziert wird
  // oder man auf "Gerät Identifizieren" klickt
  printf("accessory identify\n");
}

// Eigenschaften des Schlosses
homekit_characteristic_t cha_lock_current_state = HOMEKIT_CHARACTERISTIC_(LOCK_CURRENT_STATE, 1);
homekit_characteristic_t cha_lock_target_state = HOMEKIT_CHARACTERISTIC_(LOCK_TARGET_STATE, 1);

homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_(NAME, "Schloss");

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_door_lock, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Schloss"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "makesmart Community"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "1111111"),
            HOMEKIT_CHARACTERISTIC(MODEL, "makesmart Lock"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
    HOMEKIT_SERVICE(LOCK_MECHANISM, .primary=true, .characteristics=(homekit_characteristic_t*[]){
      &cha_lock_current_state,
      &cha_lock_target_state,
      &cha_name,
      NULL
    }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
    .accessories = accessories,
    .password = "123-45-678"
};
