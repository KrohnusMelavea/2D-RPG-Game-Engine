#include "ConsoleMagic.h"

#include <iostream>


/* Outputs a line to the console of default length 63, max length 255. */
const void outputLine(const uint8_t n)
{
    for (uint8_t i = 0; i < n; i++)
    {
        std::cout << "-";
    }
    std::cout << std::endl;
}
/* Same as regular cout, except it removes trailing breaklines at the end of the string. */
const void outputString(const std::string& text)
{
    char prev_char = '0';
    for (uint16_t i = 0; i < text.size(); i++)
    {
        if (text[i] == '\n')
        {
            if (prev_char != '\n')
            {
                std::cout << std::endl;
            }
        }
        else std::cout << text[i];
        prev_char = text[i];
    }
}
/* Outputs the source code of the two supplied shader sources with a bit of console magic sprinkled on them. */
const void outputShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    outputLine(); std::cout << std::endl; outputLine();
    std::cout << "Vertex Shader:" << std::endl; outputLine(14);
    outputString(vertex_shader);
    outputLine(45);
    std::cout << "Fragment Shader:" << std::endl; outputLine(16);
    outputString(fragment_shader);
    outputLine();
}