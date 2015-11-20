#ifndef BROKED_H
#define BROKED_H

#define BROKED_VERSION 0

#define BROKED_MAJOR_VERSION    (BROKED_VERSION / 100000)
#define BROKED_MINOR_VERSION    (BROKED_VERSION / 100 % 1000)
#define BROKED_SUBMINOR_VERSION (BROKED_VERSION % 100)

namespace bd {

/*! Connects to a broked server.
 *
 *  This blocks until a callback in the given subscriber closes its socket.
 *
 *  \param address The address of the broked server.
 *  \param sub A subscriber to handle events received from the server.
 */
  /*
void connect(const std::string& address, const subscriber& sub, std::string identity = "");
*/

}

#endif // BROKED_H
