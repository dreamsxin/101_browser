#ifndef _AbstractParser_h
#define _AbstractParser_h

class AbstractParser
{
public:
	/*!
	 * (Re)Initializes the passed state of the parser.
	 * Called before parsing begins
	 */
	virtual void initialize() = 0;

	/*!
	 * Returns
	 * false if additional tokens may come in
	 * true if parsing is finished
	 *
	 * IMPORTANT: If the parser does not accept the input it
	 * must return false here
	 */
	virtual bool isFinished() const = 0;

	/*!
	 * Parses the passed token and actualizes the state
	 */
	virtual void parseToken(char in_token) = 0;

	/*!
	 * Returns whether a when we terminate parsing now
	 * the parser would accept the input
	 */
	virtual bool acceptWhenTerminate() const = 0;
};

#endif
