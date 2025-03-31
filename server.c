#include"minitalk.h"

static int	g_x = 0; // Global variable used to track whether the client PID has been received.

/*
 *  This function reconstructs the client's PID (process ID) from the received characters
 *  and sends a confirmation signal (SIGUSR1) back to the client when the PID is complete.
 *
 *  nb  - The digit received (converted from character to integer).
 *  bol - A flag indicating whether the full PID has been received.
 */
void	take_client_pid(int nb, int bol)
{
	static int	result = 0; // Stores the reconstructed client PID.

	if (nb >= 0)
	{
		result *= 10; // Shift the existing number to the left (multiply by 10).
		result += (nb - '0'); // Convert the received character into a digit and add it.
	}
	if (bol) // If bol is set, the full PID is received.
	{
		kill(result, SIGUSR1); // Send confirmation signal to the client.
		g_x = 1; // Set global flag indicating PID has been received.
		result = 0; // Reset the PID storage variable.
	}
}

/*
 *  This function handles SIGUSR1 and SIGUSR2 signals.
 *  It reconstructs characters from bits and determines whether it's part of the message
 *  or the client's PID.
 *
 *  signum - The received signal (SIGUSR1 or SIGUSR2).
 */
void	signal_handler(int signum)
{
	static int	str1 = 0; // Stores the reconstructed character (8 bits).
	static int	i = 0; // Bit counter.

	if (signum == SIGUSR1)
		str1 = str1 | 1; // Set the least significant bit if SIGUSR1 is received.
	if (++i == 8) // When 8 bits are received, a full character is formed.
	{
		if (str1 == 27) // Special case: If character 27 is received, process PID.
			take_client_pid (-1, 1);
		else if (g_x) // If PID has been received, print the character.
			write (1, &str1, 1); 
		else if (!g_x) // If PID has not been received yet, store digits.
			take_client_pid(str1, 0);
		if (!str1) // If null character ('\0') is received, reset the flag and print a newline.
		{
			g_x = 0;
			write(1, "\n", 1);
		}
		str1 = 0; // Reset character storage.
		i = 0; // Reset bit counter.
	}	
	else
		str1 <<= 1; // Shift bits left to make space for the next one.
}

/*
 *  Initializes the server process, prints its PID, and waits for incoming signals.
 */
int	main(void)
{
	write(1, "serverPID: ", 12); // Print server PID label.
	ft_putnbr(getpid()); // Print server's process ID.
	write(1, "\n", 1);
	signal(SIGUSR1, signal_handler); // Register signal handlers for SIGUSR1.
	signal(SIGUSR2, signal_handler); // Register signal handlers for SIGUSR2.
	while (1) // Keep the server running, waiting for signals.
		pause ();
	return (0);
}