#pragma once

#define MESSAGE_LINES_LENGTH 32

struct {
    char* name;
    char* value;
} typedef MessagePlaceholder;

struct {
    int length;
    char* lines[MESSAGE_LINES_LENGTH];
} typedef Message;

struct {
    Message help;
    Message not_found;
    Message create_no_project_name;
    Message create_invalid_project_name;
} typedef Messages;


// Gets messages out of a yaml file
Messages schema_getMessages(const char* filePath);

// Prints a message to the standard output.
void schema_printMessage(Message message);

// Will print a given Message with the
// variables replaced by the definitions in variables.
void schema_printMessageWithVariables(
    const MessagePlaceholder variables[],
    int variablesCount,
    Message message);
