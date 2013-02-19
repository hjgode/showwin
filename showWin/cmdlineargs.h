#ifndef CMDLINEARGS_H
#define CMDLINEARGS_H

#include <windows.h>
#include <vector>

class CmdLineArgs : public std::vector<TCHAR*>
{
public:
    CmdLineArgs ()
    {
        // Save local copy of the command line string, because
        // ParseCmdLine() modifies this string while parsing it.
        TCHAR* cmdline = GetCommandLine();
        m_cmdline = new TCHAR [_tcslen (cmdline) + 1];
        if (m_cmdline)
        {
            _tcscpy (m_cmdline, cmdline);
            ParseCmdLine(); 
        }
    }
    ~CmdLineArgs()
    {
        delete []m_cmdline;
    }

private:
    TCHAR* m_cmdline; // the command line string

    ////////////////////////////////////////////////////////////////////////////////
    // Parse m_cmdline into individual tokens, which are delimited by spaces. If a
    // token begins with a quote, then that token is terminated by the next quote
    // followed immediately by a space or terminator.  This allows tokens to contain
    // spaces.
    // This input string:     This "is" a ""test"" "of the parsing" alg"o"rithm.
    // Produces these tokens: This, is, a, "test", of the parsing, alg"o"rithm
    ////////////////////////////////////////////////////////////////////////////////
    void ParseCmdLine ()
    {
        enum { TERM  = TEXT('\0'),
               QUOTE = TEXT('\"') };

        bool bInQuotes = false;
        TCHAR* pargs = m_cmdline;

        while (*pargs)
        {
            while (iswspace (*pargs))        // skip leading whitespace
                pargs++;

            bInQuotes = (*pargs == QUOTE);  // see if this token is quoted

            if (bInQuotes)                  // skip leading quote
                pargs++; 

            push_back (pargs);              // store position of current token

            // Find next token.
            // NOTE: Args are normally terminated by whitespace, unless the
            // arg is quoted.  That's why we handle the two cases separately,
            // even though they are very similar.
            if (bInQuotes)
            {
                // find next quote followed by a space or terminator
                while (*pargs && 
                      !(*pargs == QUOTE && (iswspace (pargs[1]) || pargs[1] == TERM)))
                    pargs++;
                if (*pargs)
                {
                    *pargs = TERM;  // terminate token
                    if (pargs[1])   // if quoted token not followed by a terminator
                        pargs += 2; // advance to next token
                }
            }
            else
            {
                // skip to next non-whitespace character
                while (*pargs && !iswspace (*pargs)) 
                    pargs++;
                if (*pargs && iswspace (*pargs)) // end of token
                {
                   *pargs = TERM;    // terminate token
                    pargs++;         // advance to next token or terminator
                }
            }
        } // while (*pargs)
    } // ParseCmdLine()
}; // class CmdLineArgs


#endif // CMDLINEARGS_H