Hello ninja and GYP
My English is not good to describe what is ninja and GYP, so I just list all steps that building a 'hello world' with these tools on Ubuntu.

1. Install ninja
    sudo apt-get install ninja

2. Install GYP
    sudo apt-get install gyp

3. Create a C file
    vi hello.c

with following content
#include <stdio.h>

int main(void)

{

    printf("Hello, world!\n");

    return 0;

}

4. Create GYP file for generating build.ninja or Makefile
    vi hello.gyp

with following content
{

    'targets': [

    {

        'target_name': 'hello',

            'type': 'executable',

            'sources': [

                'hello.c'

            ],

    },

    ],

}

5. Generate build.ninja file
    gyp hello.gyp --depth=. --generator-output=release -f ninja

    for generate Makefile
    gyp hello.gyp --depth=. --generator-output=release

6. Build with ninja
    ninja -C ./release/out/Default/ all

7. Execute
    ./release/out/Default/hello

8. You'll see output
Hello, world!
