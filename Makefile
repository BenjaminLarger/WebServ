# Color
GREEN=\033[0;32m
RED=\033[0;31m
YELLOW=\033[0;33m
BLUE=\033[0;34m
BLUE_BOLD=\033[1;34m
NC=\033[0m

# Compiler and tools
CC=gcc
CXX=g++
RM=rm -f
MKDIR_P=mkdir -p

# Compilation flags
CFLAGS=-g -Wall -Wextra -Werror -std=c++98 #-fsanitize=address

# Source files and object files
SRCS=main.cpp
OBJS=$(addprefix build/,$(SRCS:.cpp=.o))
DEPS=$(OBJS:.o=.d)

# Output binary name
NAME=a.out

.PHONY: all clean distclean

# Targets
all: build $(NAME)
	@echo "${BLUE_BOLD}🚀 $(NAME) Built successful! 🚀${BLUE_BOLD}"

build:
	@echo -e "${YELLOW}📁 Creating build directory...${NC}"
	$(MKDIR_P) build
	@echo -e "${GREEN}📁 Build directory created.${NC}"

$(NAME): $(OBJS)
	@echo -e "${YELLOW}🔗 Linking object files into $(NAME)...${NC}"
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo -e "${GREEN}🔥 Executable $(NAME) created.${NC}"

# Automatically generate dependencies
-include $(DEPS)

build/%.o: %.cpp
	@echo -e "${YELLOW}🔨 Compiling $<...${NC}"
	$(CXX) $(CFLAGS) -MMD -MP -c $< -o $@
	@echo -e "${GREEN}✅ Compiled $< to $@.${NC}"

clean:
	@echo -e "${RED}🧹 Cleaning up...${NC}"
	$(RM) $(OBJS) $(DEPS)
	@echo -e "${RED}🧹 Clean up completed.${NC}"


distclean: clean
	$(RM) *~ .depend

fclean:
	@echo -e "${RED}🧹 Performing a deep clean...${NC}"
	$(RM) $(OBJS) $(DEPS)
	$(RM) *~ .depend
	$(RM) $(NAME)
	rm -rf build/
	@echo -e "${RED}🧹 Deep clean completed.${NC}"

re: fclean all