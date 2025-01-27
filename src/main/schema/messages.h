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
} typedef Messages;

Messages schema_getMessages(const char* filePath);

void schema_printMessage(Message message);

void schema_printMessageWithVariables(
    const MessagePlaceholder variables[],
    int variablesCount,
    Message message);
