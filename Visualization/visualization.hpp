#ifndef VISUALIZATION_HPP
#define VISUALIZATION_HPP

#include "infoAcquisition.hpp"

/**
 * @brief Displays the process information
 * 
 * @param processes The processes of the system
 * @param number_of_processes The number of processes displayed
 * @param arguments How the processes are going to be displayed
 */
void DisplayProcessInfo(std::vector<ProcessInfo>& processes, unsigned number_of_processes,
    std::vector<int> arguments);

/**
 * @brief Displays CPU usage
 * 
 * @param process The process whose information will be displayed
 */
void displayCPUUsage(struct ProcessInfo process);

/**
 * @brief Displays net usage
 * 
 * @param process The process whose information will be displayed
 */
void displayNetUsage(struct ProcessInfo process);

/** Function to get key without waiting
 * @param timeoutMs timeout in milliseconds
 * @returns 0 if no key is pressed
 **/
char getKey(int timeoutMs);

/**
 * @param msToCharge time in milliseconds to charge the bar
 * @returns 0 if no key is pressed
*/
int DisplayBar(int msToCharge = 4000);

#endif