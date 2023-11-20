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
void DisplayProcessInfo(std::vector<ProcessInfo>& processes, Arguments& args, int sort_counter);

/**
 * @brief Displays CPU usage
 * 
 * @param process The process whose information will be displayed
 */
void displayCPUUsage(struct ProcessInfo& process);

/**
 * @brief Displays network usage
 * 
 * @param process The process whose information will be displayed
 */
void displayNetUsage(struct ProcessInfo& process);

/**
 * @brief Displays RAM usage of a process
 * 
 * @param process The process whose information will be displayed
 */
void displayRAMUsage(struct ProcessInfo& process);

/**
 * @brief Displays the real user of the process
 * 
 * @param process The process from which retrieve the data
 */
void displayUser(struct ProcessInfo& process);

/**
 * @brief Displays the real group of the process
 * 
 * @param process The process from which retrieve the data
 */
void displayGroup(struct ProcessInfo& process);

/**
 * @brief Displays the uptime of the process
 * 
 * @param process The process from which retrieve the data
 */
void displayUptime(struct ProcessInfo& process);

void displayIOStats(struct ProcessInfo& process);

/** Function to get key without waiting
 * @param timeoutMs timeout in milliseconds
 * @returns 0 if no key is pressed
 */
char getKey(int timeoutMs);

/**
 * @param msToCharge time in milliseconds to charge the bar
 * @returns 0 if no key is pressed
*/
int DisplayBar(int msToCharge = 4000);

/**
 * @brief Draws an horizontal bar for separation
 * 
 */
void drawHorizontalBar(std::vector<int> args);

/**
 * @brief Shows the pre-header
 * 
 */
void showPreHeader(const std::vector<ProcessInfo>& processes, Arguments& args);


#endif