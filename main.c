#include "mgos.h"
#include "stdio.h"

#include "esp32_bt.h"
#include "esp32_bt_gattc.h"
#include "esp32_bt_gatts.h"
#include "esp32_bt_internal.h"

#include "mgos_bt.h"
#include "mgos_bt_gap.h"
#include "mgos_bt_gattc.h"
#include "mgos_event.h"


static void logging_data_cb(int ev, void*ev_data, void *data) {
  struct mgos_bt_gap_scan_result *ble_data  = ev_data;
  char addr[17];
  void *address;
  struct mg_str manufacturer;
  
  address = &ble_data->addr;

  // You can get any data on the advertising content with the function mgos_bt_gap_parse_adv_data(data, enum)
  manufacturer = mgos_bt_gap_parse_adv_data(ble_data->adv_data, MGOS_BT_GAP_EIR_FULL_NAME);  
  printf("\nNAME: %.*s",  manufacturer.len, manufacturer.p);
  printf("\nADDR: %s", mgos_bt_addr_to_str(address, addr));
  // I'm printing the manufacturer data on hexadecimals because the characters 
  // I'm using are not in the ASCII table, just for printing purpose. (I already know their position and that it's size is only 2)
  printf("\nADV_DATA: %x%x", ble_data->adv_data[22], ble_data->adv_data[23]);
  printf("\nRSSI: %d\n", ble_data->rssi);

}

static void timer_cb(void *arg){
    printf("\nBluetooth Scanning Starting!!\n");
    struct mgos_bt_gap_scan_opts period = {1000};
    mgos_bt_gap_scan(&period);
    (void) arg;
}

enum mgos_app_init_result mgos_app_init(void) {
      struct mgos_bt_gap_scan_result *result;
      mgos_event_add_handler(MGOS_BT_GAP_EVENT_SCAN_RESULT, logging_data_cb, &result);
      //You can also send the period of mgos_bt_gap_scan function here, I'll be updating it later.
      mgos_set_timer(60000, MGOS_TIMER_REPEAT, timer_cb, NULL);
    return MGOS_APP_INIT_SUCCESS;
}
