SHELL		= /bin/sh

# Output binary name
NAME		:= webserv

# Compiler and tools
CC			= clang
CXX			= clang++
RM			= rm -f
MKDIR_P	= mkdir -p

# Compilation flags
CFLAGS	= -g -Wall -Wextra -Werror -std=c++98 -I $(HEADER_DIR) \
	-Wshadow -Wunreachable-code -fsanitize=address,undefined,leak

# Header files directory
HEADER_DIR	= inc/

# Source files and object files
SRCS_DIR		= src/
SRC_FILES		= main.cpp ClientProcessMethods.cpp Webserv.cpp \
	ErrorUtils.cpp utils.cpp core.cpp \
	ServerConfig.cpp ServerConfigParsing.cpp ServerConfigParsingLocation.cpp \
	ClientInfo.cpp \
	GET.cpp GETdirectoryListing.cpp CGI.cpp POST.cpp POSTupload.cpp  POSTutils.cpp\
	WebservCreateServers.cpp WebservClientRequest.cpp WebservNewConnection.cpp WebservSignal.cpp \
	ManageWriteReadClientCapability.cpp HttpExceptions.cpp \
#	ConfigurationFile.cpp WebservUtils.cpp ConfigurationFileParsing.cpp

SRCS				= $(addprefix $(SRCS_DIR),$(SRC_FILES))

BUILD_DIR	:= build/

# Objects
OBJS				= $(addprefix $(BUILD_DIR),$(notdir $(SRCS:.cpp=.o)))
DEPS				= $(OBJS:.o=.d)


.PHONY	: all, clean, fclean, re
.DEFAULT_GOAL := all

# Targets
all			: build $(NAME)
	@echo "${BLUE_BOLD}🚀 $(NAME) Built successful! 🚀${BLUE_BOLD}"

build		:
	@echo "${YELLOW}📁 Creating build directory...${NC}"
	$(MKDIR_P) $(BUILD_DIR)
	@echo "${GREEN}📁 Build directory created.${NC}"

$(NAME)	: $(OBJS)
	@echo "${YELLOW}🔗 Linking object files into $(NAME)...${NC}"
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "${GREEN}🔥 Executable $(NAME) created.${NC}"

# Automatically generate dependencies
-include $(DEPS)

$(OBJS)	: $(BUILD_DIR)%.o : $(SRCS_DIR)%.cpp
	@echo "${YELLOW}Compiling $< into $@...${NC}"
	$(CXX) $(CFLAGS) -c $< -o $@

clean		:
	@echo "${RED}🧹 Cleaning up...${NC}"
	$(RM) $(OBJS) $(DEPS)
	rm -rf $(BUILD_DIR)
	@echo "${RED}🧹 Clean up completed.${NC}"

fclean	:
	@echo "${RED}🧹 Performing a deep clean...${NC}"
	$(RM) $(OBJS) $(DEPS)
	rm -rf $(BUILD_DIR)
	$(RM) $(NAME)
	@echo "${RED}🧹 Deep clean completed.${NC}"

re			: fclean all


# Color
GREEN=\033[0;32m
RED=\033[0;31m
YELLOW=\033[0;33m
BLUE=\033[0;34m
BLUE_BOLD=\033[1;34m
NC=\033[0m