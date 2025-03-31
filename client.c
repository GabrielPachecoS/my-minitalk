#include "minitalk.h"

/*
 * Sends a binary string as signals to the server.
 * Each '0' is sent as SIGUSR2 and each '1' as SIGUSR1.
 */
void	send_binary_signal(char *binary, int sver_pid)
{
	int	i;

	i = 0;
	while (binary[i]) // Iterate through each bit in the binary string.
	{
		if (binary[i] == '0')
			kill(sver_pid, SIGUSR2); // Send SIGUSR2 for bit '0'
		else if (binary[i] == '1')
			kill(sver_pid, SIGUSR1); // Send SIGUSR1 for bit '1'.
		usleep(1000); // Delay to ensure signals are received properly.
		i++;
	}
}

/*
 * Converts the client's PID to binary and sends it to the server.
 * This helps the server know which client sent the message.
 */
void	send_client_pid(int server_pid)
{
	char	*cpid;
	int		i;
	char	*c_binary;
	int		x;

	i = 0;
	cpid = itoa_pid(getpid()); // Convert client PID to a string.
	while (cpid[i]) // Iterate through each character of the PID.
	{
		c_binary = convertbinary(cpid[i]); // Convert each character to binary.
		x = 0;
		while (c_binary[x]) // Send each bit of the binary representation.
		{
			if (c_binary[x] == '0')
				kill(server_pid, SIGUSR2); // Send '0' as SIGUSR2.
			else if (c_binary[x] == '1')
				kill(server_pid, SIGUSR1); // Send '1' as SIGUSR1.
			usleep(1000);  // Shorter delay since PID is small.
			x++;
		}
		free(c_binary); // Free allocated memory for binary representation.
		i++;
	}
	free(cpid); // Free allocated memory for PID string.
}

/*
 * Handles confirmation signals from the server.
 * If SIGUSR1 is received, it prints "SUCCESS!".
 */
void	message(int signum)
{
	if (signum == SIGUSR1)
		write(1, "SUCCESS!\n", 10); // Confirmation that the server received the message.
}

int	main(int argc, char **argv)
{
	int		i; // Variable used as an index to iterate through each character in the message string.
	int		len; // Variable that holds the length of the message string to control the loop.
	char	*binary; // Pointer to hold the binary representation of each character in the message.

	i = 0;
	signal(SIGUSR1, message);// Register the signal handler to listen for SIGUSR1 from the server.
	if (argc == 3)
	{
		send_client_pid(ft_atoi(argv[1])); // Convert the server's PID and send the client's PID to the server.
		len = ft_strlen(argv[2]); // Get the length of the message string.
		while (len >= i)
		{
			binary = convertbinary(argv[2][i]); // Convert the current character in the message to its binary representation.
			send_binary_signal(binary, ft_atoi(argv[1])); // Send the binary string bit by bit to the server using signals.
			free(binary); // Free the memory allocated for the binary string after sending it.
			i++;  // Increment the index i to move to the next character in the message string.
		}
	}
	else
		write(1 ,"ERROR\n", 7); // If the number of arguments is incorrect, print "ERROR".
	return (0); // Return 0 to indicate that the program has completed successfully.
}