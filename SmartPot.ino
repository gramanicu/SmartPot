/**
 * @file SmartPot.ino
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The main code used by the SmartPot
 * @copyright Copyright (c) 2021
 */

#include "libraries/Comms.hpp"
#include "libraries/Display.hpp"
#include "libraries/Pump.hpp"
#include "libraries/Sensors.hpp"

using namespace SmartPot;
namespace SmartPot::DefaultValues {
    const uint32_t default_soil_trigger = 30;
    const uint32_t default_water_trigger = 10;
    const uint32_t default_light_trigger = 40;
    const uint32_t default_pump_speed = 75;
    const uint32_t default_pump_duration = 5000;
    const uint64_t default_logic_delay = 43200;
}    // namespace SmartPot::DefaultValues

Display display;
Pump pump;
Sensors sensors;
Comms comm;

// The millis() when the last "delay" was "accomplished". This is
// related to the "logic" of the system.
uint64_t last_time = 0;

// Settings
int threshold_soil =
    DefaultValues::default_soil_trigger;    // The "lower bound" for the soil
                                            // moisture level
int threshold_water =
    DefaultValues::default_water_trigger;    // The point at which the water
                                             // reservoir needs to be refilled
int threshold_light =
    DefaultValues::default_light_trigger;    // The point at which the light is
                                             // considered to be enough

bool light_check = true;
bool text_warnings = true;
bool values_print = true;
int pumping_duration = DefaultValues::default_pump_duration;
int pumping_speed = DefaultValues::default_pump_speed;
int logic_delay = DefaultValues::default_logic_delay;

void setup() {
    display = Display();
    comm = Comms();
    pump = Pump(&display, &comm);
    sensors = Sensors(&display, &comm);
}

/**
 * @brief Read the data from the sensors, output the information and
 * (optionally) act on in.
 * @param check_only Only show, do not use the data to "take action" (pump)
 */
void read_sensors(bool check_only) {
    Sensors::SensorsData data = sensors.read_all();

    // Print sensors data
    String info = String("WL:") + data.water + "%; SM:" + data.soil + "%";
    bool refill = false, pump = false;

    // Evaluate the data
    String message("");
    if (data.light < threshold_light && light_check) {
        message += "Not enough light for the plant! ";
    }
    if (data.water < threshold_water) {
        message += "Water level is too low! ";
        refill = true;
    }
    if (data.soil < threshold_soil) {
        message += "Soil is too dry! ";
        pump = true;
    }

    // Print the data and warning messages
    if (values_print) {
        display.print(info.c_str());
        comm.print(info.c_str());
    }
    if (text_warnings) {
        display.print(message.c_str(), 1);
        comm.print(message.c_str());
    }

    display.update();

    // Act on the data (if needed)
    if (!check_only) {
        if (!refill & pump) { run_pump(); }
    }
}

/**
 * @brief Show the current config on the LCD and serial comms
 */
void show_config() {
    String message("");
    message += String("LCK:") + light_check + "; ";
    message += String("TWS:") + text_warnings + "; ";
    message += String("VPS:") + values_print + "; ";
    message += String("PDU:") + pumping_duration + "ms; ";
    message += String("PDS:") + pumping_speed + "%; ";
    message += String("LTO:") + logic_delay + "ms.";

    display.print(message.c_str(), 1);
    comm.print(message.c_str());

    display.update();
}

/**
 * @brief Turn on the pump and run it, according to the current config
 */
void run_pump() {
    if (text_warnings) {
        String msg = String("Pumping at ") + pumping_speed + "% for " +
                     pumping_duration + "ms";
        display.print(msg.c_str(), 1);
        comm.print(msg.c_str());
        display.update();
    }

    pump.run(pumping_speed, pumping_duration);
}

/**
 * @brief Parse and execute commands received. All comands have 7 chars to
 * describe the command and after that, a space and the argument (only one,
 * integer) Possible commands: RUN_PMP, SET_LCK, SET_TWS, SET_VPS, SET_PDU,
 * SET_PDS, SET_LTO, SHW_CFG, SHW_SNS
 * @param line The full command string
 */
void execute_command(String line) {
    String cmd = line.substring(0, 7);
    int arg = 0;
    if (line.length() > 8) { arg = line.substring(8).toInt(); }

    if (cmd == "RUN_PMP") {
        // Run the pump, using the current pump settings
        run_pump();
    } else if (cmd == "SET_LCK") {
        // Set the "light_check" (0 or 1)
        light_check = arg;
    } else if (cmd == "SET_TWS") {
        // Set the "text_warnings" (0 or 1)
        text_warnings = arg;
    } else if (cmd == "SET_VPS") {
        // Set the sensors "values_print"
        values_print = arg;
    } else if (cmd == "SET_PDU") {
        // Set the duration the pump should run for (in ms)
        pumping_duration = arg;
    } else if (cmd == "SET_PDS") {
        // Set the speed the pump should run at (0-100)
        pumping_speed = arg;
    } else if (cmd == "SET_LTO") {
        // Set the "logic loop" timeout (in ms)
        logic_delay = arg;
    } else if (cmd == "SHW_CFG") {
        // Show the current config
        show_config();
    } else if (cmd == "SHW_SNS") {
        // Show the sensor information
        read_sensors(true);
    } else {
        comm.print(String("Received invalid command " + cmd).c_str());
    }
}

void loop() {
    // Compute the delta of the "delay". Some things should be run on a delay,
    // but it would have made some other functionalities unresponsive
    uint64_t delay_delta = millis() - last_time;

    // Read commands and execute them
    while (Serial.available()) {
        String recv = Serial.readString();
        recv.replace('\n', '\0');
        comm.print(String("Received: " + recv).c_str());
        execute_command(recv);
    }

    // When specific conditions are met
    if (delay_delta > logic_delay || last_time == 0) {
        // Analyze sensors & Water plant / display "error" messages
        read_sensors(false);

        // Save the time when this loop was completed
        last_time = millis();
    }

    // Clear the info and show only the stats
    display.clear_buffer(1);
    display.update();
}
