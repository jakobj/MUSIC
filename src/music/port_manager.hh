
#ifndef MUSIC_PORT_MANAGER_HH
#include "music/music-config.hh"

#if MUSIC_USE_MPI
#include <map>
#include <string>
#include <memory>
#include <mpi.h>
#include "music/configuration.hh"
#include "music/error.hh"
#include "music/port.hh"
#include "music/misc.hh"


namespace MUSIC
{
	// We could also use raw pointers but we do not (see application.hh why so)
	/* using PortMap = std::map<std::string, std::shared_ptr<Port>>; */
	using PortMap = std::map<std::string, std::weak_ptr<Port>>;

	class PortConnectivityManager
	{
		public:

			PortConnectivityManager (Configuration& config)
				  : config_ (config)
				  , portMap_ ()
	              , isConnectivityModified (false)
			{}

			PortConnectivityManager(const PortConnectivityManager&) = delete;
			PortConnectivityManager& operator= (const PortConnectivityManager&) = delete;


			void connect(std::string senderApp, std::string senderPort,
					std::string receiverApp, std::string receiverPort,
					int width,
					ConnectorInfo::CommunicationType commType,
					ConnectorInfo::ProcessingMethod procMethod);

			/* void disconnect (std::string identifier); */
			void disconnect (std::string appName, std::string portName);
			void disconnect (std::string senderApp, std::string senderPort, std::string receiverApp, std::string receiverPort);

			bool isConnected (std::string identifier) const;
			bool isInstantiated (std::string identifier);

			// TODO move in private
			void updatePorts();

			SPVec<Port> getPorts ();

		private:
			friend class Port;
			// TODO this can be remove and replaced by exposing iterators

			Configuration& config_;
			/* std::string applicationName_; */
			/* std::shared_ptr<ApplicationMap> applicationMap_; */
			/* std::unique_ptr<Connectivity> connectivityMap_; */
			PortMap portMap_;
			/* Application& app_; */
			bool isConnectivityModified;

			// TODO move impl to cpp file
			template <typename T>
			void addPort (std::shared_ptr<T> port)
			{
				std::shared_ptr<Port> ptr (std::static_pointer_cast<Port> (port));
				if (isInstantiated (ptr->name ()))
					error (std::string ("Port already has been instantiated."));
				portMap_.emplace (port->name (), port);
			}

			template <typename T>
		    std::shared_ptr<T> createPort (const Application& app, std::string identifier)
			{
				auto ptr (std::make_shared<T> (app, identifier));
				addPort(ptr);
				return ptr;
			}

			void removePort (std::string identifier);
			const ConnectivityInfo& portConnectivity (const std::string localName) const;



	};
}

#endif
#define MUSIC_PORT_MANAGER_HH
#endif
