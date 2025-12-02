/*
 * Copyright (c) 2020 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include "adc.h"
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <stdlib.h>


/* Bluetooth-kirjastot */
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>

#define USER_BUTTON_2           DK_BTN2_MSK

/*Alustetaan zephyr moduuli*/
LOG_MODULE_REGISTER(MAIN, LOG_LEVEL_INF); 

static int16_t direction_mode = 0;
/* BLE-palvelu kiihtyvyysdatalle, accel x,y,z ovat globaaleja muuttujia*/
static int16_t accel_x, accel_y, accel_z;

/*Alustetaan rakenteet, luodaan jokaiselle palvelulle oma 128bittinen UUID*/
/*BT_UUID_INIT_128 asettaa bt_uuid_128-rakenteet zephyrin käytettäväksi GATT:iin*/
#define BT_UUID_ACCEL_SERVICE_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)
#define BT_UUID_ACCEL_X_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1)
#define BT_UUID_ACCEL_Y_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef2)
#define BT_UUID_ACCEL_Z_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef3)

#define BT_UUID_ACCEL_DIR_VAL \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef4)

static struct bt_uuid_128 accel_dir_uuid = BT_UUID_INIT_128(BT_UUID_ACCEL_DIR_VAL);
static int16_t accel_dir = 0;    

static struct bt_uuid_128 accel_service_uuid = BT_UUID_INIT_128(BT_UUID_ACCEL_SERVICE_VAL);
static struct bt_uuid_128 accel_x_uuid = BT_UUID_INIT_128(BT_UUID_ACCEL_X_VAL);
static struct bt_uuid_128 accel_y_uuid = BT_UUID_INIT_128(BT_UUID_ACCEL_Y_VAL);
static struct bt_uuid_128 accel_z_uuid = BT_UUID_INIT_128(BT_UUID_ACCEL_Z_VAL);

/* BLE-palvelun määrittely */
/*BT_GATT_SERVICE_DEFINE rakenne sisältää palvelun ja sen ominaisuudet*/
BT_GATT_SERVICE_DEFINE(accel_svc,
    BT_GATT_PRIMARY_SERVICE(&accel_service_uuid),
    BT_GATT_CHARACTERISTIC(&accel_x_uuid.uuid,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ,
        NULL, NULL, &accel_x),
    BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(&accel_y_uuid.uuid,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ,
        NULL, NULL, &accel_y),
    BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(&accel_z_uuid.uuid,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ,
        NULL, NULL, &accel_z),
    BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

    BT_GATT_CHARACTERISTIC(&accel_dir_uuid.uuid,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ, NULL, NULL, &accel_dir),
    BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

/* BLE yhteydenhallinta */
static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err)
        printk("Yhteys epäonnistui (err %u)\n", err);
    else
        printk("Bluetooth-yhteys muodostettu\n");
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Bluetooth-yhteys katkaistu (syy %u)\n", reason);
}

/*Rekisteröidään callbackit staattisesti*/
BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnected,
};

/* Globaalisti */
/*Mainostus connectable-tilassa, lähettää oman nimensä*/
/*BT_LE_AD_NO_BREDR tarkoittaa että käytössä on vain low energy-yhteys*/
static const struct bt_data ad[] = {
    BT_DATA(BT_DATA_FLAGS, (uint8_t[]){BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR}, 1),
    BT_DATA(BT_DATA_NAME_COMPLETE, (uint8_t *)CONFIG_BT_DEVICE_NAME,
            strlen(CONFIG_BT_DEVICE_NAME))
};

void ble_init(void)
{
    int err;

    printk("Alustetaan Bluetooth...\n");
/*Käynnistetään ble-pino*/
    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init epäonnistui (err %d)\n", err);
        return;
    }

    settings_load();
    
    printk("Bluetooth käynnistetty onnistuneesti\n");

    /* Käynnistetään mainostus connectable + device name */
    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Mainostus epäonnistui (err %d)\n", err);
        return;
    }

    printk("Mainostus käynnissä (nRF Connect näkee laitteen)\n");
}

void ble_update_accel(int16_t dir, int16_t x, int16_t y, int16_t z)
{
    /* Päivitetään globaalit muuttujat*/
    accel_dir = dir;
    accel_x = x;
    accel_y = y;
    accel_z = z;

    /* Lähetetään ilmoitukset */
    bt_gatt_notify(NULL, &accel_svc.attrs[2], &accel_x, sizeof(accel_x));
    bt_gatt_notify(NULL, &accel_svc.attrs[5], &accel_y, sizeof(accel_y));
    bt_gatt_notify(NULL, &accel_svc.attrs[8], &accel_z, sizeof(accel_z));
    bt_gatt_notify(NULL, &accel_svc.attrs[11], &accel_dir, sizeof(accel_dir));
}

/* Nappien callback */
static void button_changed(uint32_t button_state, uint32_t has_changed)
{
    if ((has_changed & USER_BUTTON_2) && (button_state & USER_BUTTON_2)) {
        direction_mode++;
        if (direction_mode > 5)
            direction_mode = 0;
        printk("Nappi 2: suunta-mode = %d\n", direction_mode);
    }
}

/* Nollakalibrointi */
static struct Measurement zero = {0};

void calibrate(void)
{
    printk("Kalibrointi käynnissä: aseta lauta pöydälle...\n");
    k_sleep(K_MSEC(1000));

    zero = readADCValue();

    printk("Kalibrointi valmis: x=%d y=%d z=%d\n", zero.x, zero.y, zero.z);
}

/*Määritellään deadzone kohinan suodattamiseksi jotta tulokset ei heittele niin paljoa*/
#define DEADZONE 100 

/* Suunnan laskenta absoluuttisten muutoksien vertailulla */
int calculate_direction(struct Measurement *m)
{
    int dx = m->x - zero.x;
    int dy = m->y - zero.y;
    int dz = m->z - zero.z;

    int abs_dx = abs(dx);
    int abs_dy = abs(dy);
    int abs_dz = abs(dz);

    if (abs_dx < DEADZONE &&
        abs_dy < DEADZONE &&
        abs_dz < DEADZONE) {
        return 0; 
    }
    if (abs_dx > abs_dy && abs_dx > abs_dz)
        return (dx > 0) ? 1 : 2; // X ylös / alas
    else if (abs_dy > abs_dx && abs_dy > abs_dz)
        return (dy > 0) ? 3 : 4; // Y ylös / alas
    else
        return (dz > 0) ? 5 : 6; // Z ylös / alas
}

void main(void)
{
	int err;

	err = dk_buttons_init(button_changed);
	if (err) {
		printk("Cannot init buttons (err: %d)\n", err);
		return;
	}
	
	if (initializeADC() != 0) {
		printk("ADC initialization failed!");
		return;
	}
	/* Kalibrointi pöydällä */
	calibrate();
	/* BLE käynnistys */
	ble_init();
	printk("ADC + BLE demo käynnissä...\n");

	while (1) 
	{
		struct Measurement m = readADCValue();
		m.direction = calculate_direction(&m);

		/* Lähetetään BLE:n yli */
        ble_update_accel(direction_mode, m.x, m.y, m.z);

        printk("x = %d, y = %d, z = %d, suunta(auto) = %d, suunta(manual) = %d\n",
           m.x, m.y, m.z, m.direction, direction_mode);
		k_sleep(K_MSEC(1000));
	}
}