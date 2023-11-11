#ifndef VISUALIZATION_HPP
#define VISUALIZATION_HPP

#include "infoAcquisition.hpp"


void DisplayProcessInfo(std::vector<ProcessInfo>& processes, unsigned number_of_processes,
    std::vector<int> arguments);

void displayCPUUsage(struct ProcessInfo process);

void displayCNetUsage(struct ProcessInfo process);

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