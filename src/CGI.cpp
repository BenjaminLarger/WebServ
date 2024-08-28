/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blarger <blarger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:38:48 by demre             #+#    #+#             */
/*   Updated: 2024/08/28 15:02:27 by blarger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "core.hpp"

extern char **environ;

static void checkFileAndScriptExecPaths(std::string const &filePath,
                                        std::string const &script)
{
  // Check for PHP and python executables
  if (script == "php")
  {
    if (access("/usr/bin/php", F_OK) == -1)
      throw HttpException(500, "PHP executable not found at /usr/bin/php");
  }
  else if (script == "py")
  {
    if (access("/usr/bin/python3", F_OK) == -1)
      throw HttpException(500,
                          "Python executable not found at /usr/bin/python3");
  }
  else
    throw HttpException(500, "Unsupported script type to execute: " + script);

  // Check if script file to execute exists
  if (access((char *)filePath.c_str(), F_OK) == -1)
    throw HttpException(404,
                        "Script file to execute not found at: " + filePath);
}

static void setEnvironmentVariables(std::string const &filePath,
                                    std::string const &queryString)
{
  if (!queryString.empty())
  {
    if (setenv("QUERY_STRING", queryString.c_str(), 1) == -1)
      throw HttpException(500, "Failed to setenv QUERY_STRING");
  }

  if (setenv("PATH_INFO", filePath.c_str(), 1) == -1)
    throw HttpException(500, "Failed to setenv PATH_INFO");
}

void Webserv::executeScript(std::string const &filePath,
                            std::string const &script,
                            std::string const &queryString, int &clientFD)
{
  std::cout << "Executing: " << filePath;
  if (queryString.size())
    std::cout << " + " << queryString;
  std::cout << std::endl;

  setenv("PATH_INFO", filePath.c_str(), 1);

  checkFileAndScriptExecPaths(filePath, script);

  int pipefd[2];
  if (pipe(pipefd) == -1)
    throw HttpException(500, "Pipe error when executing " + filePath);

  pid_t pid = fork();
  if (pid == -1)
    throw HttpException(500, "Fork error when executing " + filePath);

  if (pid == 0) // Execute script in child process
  {
    if (close(pipefd[0]) == -1 || dup2(pipefd[1], STDOUT_FILENO) == -1
        || close(pipefd[1]) == -1)
      exit(1);

    setEnvironmentVariables(filePath, queryString);

    if (script == "php")
    {
      char *argv[] = {(char *)"php", (char *)filePath.c_str(), NULL};
      execve("/usr/bin/php", argv, environ);
    }
    else if (script == "py")
    {
      char *argv[] = {(char *)"python3", (char *)filePath.c_str(), NULL};
			
			dup2(pipefd[0], STDIN_FILENO);
      execve("/usr/bin/python3", argv, environ);
    }
    exit(1);
  }
  else // Add read end
  {
    close(pipefd[1]);

    setNonBlocking(pipefd[0]);

    // Store the pid to manage multiple processes
    pidMap[pipefd[0]] = pid;

    pollfd pfd;
    pfd.fd = pipefd[0];
    pfd.events = (POLLIN | POLLHUP);
    pfd.revents = 0;

    // Associate the pipe FD with the client connection
    clientScriptMap[pipefd[0]] = clientFD;
 		std::cout << GREEN << "clientScriptMap[" << pipefd[0] << "] = " << clientFD << RESET << std::endl;
    fds.push_back(pfd);
		std::cout << "cgi : fds.size() = " << fds.size() << std::endl;
		
    // Add a dummy client info for the listening socket
    ClientInfo ci;
    ci.socketFD = pipefd[0];
    ci.port = -1;
    clients.push_back(ci);
  }
}

std::string	getPostData(std::string &clientInput)
{
	std::string line;
	std::string lastLine;
	std::istringstream stream(clientInput);

	while (std::getline(stream, line))
		lastLine = line;

	std::cout << GREEN << "Last line : " << lastLine << RESET << std::endl;
	return (lastLine);
}

char **getExceveEnvp(ClientInfo &client)
{
	std::vector<std::string> envVars;

	if (client.req.fields.find("Content-Length") != client.req.fields.end())
	{
		std::string contentLength = "CONTENT_LENGTH=" + client.req.fields["Content-Length"];
		envVars.push_back(contentLength);
	}

	if (client.req.fields.find("Content-Type") != client.req.fields.end())
	{
		std::string contentType = "CONTENT_TYPE=" + client.req.fields["Content-Type"];
		envVars.push_back(contentType);
	}

	std::string requestMethod = "REQUEST_METHOD=POST";
	envVars.push_back(requestMethod);
	
	// Convert std::vector<std::string> to char**
	char **env = new char*[envVars.size() + 1];
	for (size_t i = 0; i < envVars.size(); ++i)
	{
		env[i] = new char[envVars[i].size() + 1];
		std::strcpy(env[i], envVars[i].c_str());
		std::cout << GREEN << envVars[i] << RESET << std::endl;
	}
	env[envVars.size()] = NULL;

	return (env);
}

char **getExceveArgs(std::string &filePath)
{
	std::vector<std::string> ArgsVars;

	std::string pythonPath = "/usr/bin/python3";
	ArgsVars.push_back(pythonPath);
	ArgsVars.push_back(filePath);
	// Convert std::vector<std::string> to char**
	char **env = new char*[ArgsVars.size() + 1];//TO FREE!
	for (size_t i = 0; i < ArgsVars.size(); ++i)
	{
		env[i] = new char[ArgsVars[i].size() + 1];
		std::strcpy(env[i], ArgsVars[i].c_str());
	}
	env[ArgsVars.size()] = NULL;

	return (env);
}

void	deleteCharArray(char **envp)
{
	int	i = 0;

	while (envp[i])
	{
		delete[] envp[i];
		i++;
	}
	delete[] envp;
}
void Webserv::executeScript(std::string &filePath,
                            std::string const &script,
														ClientInfo &client)
{
  std::cout << "Executing: " << filePath << std::endl;

		// Arguments for execve
	char **envp = getExceveEnvp(client);
	char **argv = getExceveArgs(filePath);
		// Example POST data
	std::string	post_data = getPostData(client.req.buffer);
	size_t post_data_len = strlen(post_data.c_str());
  checkFileAndScriptExecPaths(filePath, script);
	std::cout << GREEN << "Valid script" << RESET << std::endl;

  int pipefd[2];
	int output_pipefd[2];
  if (pipe(pipefd) == -1 || pipe(output_pipefd) == -1)
    throw HttpException(500, "Pipe error when executing " + filePath);

  pid_t pid = fork();
  if (pid == -1)
    throw HttpException(500, "Fork error when executing " + filePath);

  if (pid == 0) // Execute script in child process
  {
    if (close(pipefd[1]) == -1 || dup2(pipefd[0], STDIN_FILENO) == -1
        || close(pipefd[0]) == -1)
      exit(1);

		// Close the read end of the output pipe
		// Redirect stdout to the write end of the output pipe
		if (close(output_pipefd[0]) == -1 || dup2(output_pipefd[1], STDOUT_FILENO) == -1
			|| close(output_pipefd[1]) == -1)
			exit(1);


		// Execute the script
		execve(filePath.c_str(), argv, envp);
		dprintf(2, "Failed to execute script\n");
		//dprintf(2, "failed to exec script...\n");
    exit(1);
  }
  else // Add read end of the pipe to the pollfd vector in parent process
  {
    if (close(pipefd[0]) == -1 || write(pipefd[1], post_data.c_str(), post_data_len) == -1
			|| close(pipefd[1]) == -1 || close(output_pipefd[1]) == -1)
			throw HttpException(500, "Pipe error when executing " + filePath);

		// Read the output from the read end of the output pipe
		std::vector<char> buffer(2048);
		ssize_t bytes_read;
		while ((bytes_read = read(output_pipefd[0], buffer.data(), buffer.size())) > 0)
				client.response.insert(client.response.end(), buffer.data(), buffer.data() + bytes_read);
		close(output_pipefd[0]);
		int status;
		waitpid(pid, &status, 0);
		//std::cout << "Captured output: " << std::string(client.response) << std::endl;
		deleteCharArray(envp);
		deleteCharArray(argv);
		//make the copy into string result
  }
}