#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "xtimer.h"
#include "net/wot/coap.h"
#include "shell.h"

#define MAIN_QUEUE_SIZE (4)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static const shell_command_t shell_commands[] = {
        { NULL, NULL, NULL }
};

int main(void) {
    xtimer_sleep(3);
    /* for the thread running the shell */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    //Todo: Implement auto init
    wot_td_coap_server_init();

    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}