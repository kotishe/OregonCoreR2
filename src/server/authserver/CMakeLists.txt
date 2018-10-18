# This file is part of the OregonCore Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB_RECURSE sources_authentication Authentication/*.cpp Authentication/*.h)
file(GLOB_RECURSE sources_realms Realms/*.cpp Realms/*.h)
file(GLOB_RECURSE sources_server Server/*.cpp Server/*.h)
file(GLOB_RECURSE sources_localdir *.cpp *.h)

if ( USE_COREPCH )
	include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

if ( USE_COREPCH AND MSVC )
	set(authserver_SRCS
		PrecompiledHeaders/authPCH.cpp
		PrecompiledHeaders/authPCH.h
)
endif()

set(authserver_SRCS
  ${sources_authentication}
  ${sources_realms}
  ${sources_server}
  ${sources_localdir}
)

if( WIN32 )
set(authserver_SRCS
    ${authserver_SRCS}
    OregonRealm.rc
)
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/server/shared
  ${CMAKE_SOURCE_DIR}/src/server/shared/Configuration
  ${CMAKE_SOURCE_DIR}/src/server/shared/Database
  ${CMAKE_SOURCE_DIR}/src/server/shared/Debugging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic/LinkedReference
  ${CMAKE_SOURCE_DIR}/src/server/shared/Dynamic/CountedReference
  ${CMAKE_SOURCE_DIR}/src/server/shared/Packets
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography
  ${CMAKE_SOURCE_DIR}/src/server/shared/Cryptography/Authentication
  ${CMAKE_SOURCE_DIR}/src/server/shared/Logging
  ${CMAKE_SOURCE_DIR}/src/server/shared/Threading
  ${CMAKE_SOURCE_DIR}/src/server/shared/Utilities
  ${CMAKE_SOURCE_DIR}/src/server/shared/Policies
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Authentication
  ${CMAKE_CURRENT_SOURCE_DIR}/Realms
  ${CMAKE_CURRENT_SOURCE_DIR}/Server
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

set(authserver_LINK_FLAGS "")

add_executable(authserver
  ${authserver_SRCS}
)

add_dependencies(authserver revision.h)

if ( NOT WIN32 )
	set_target_properties(authserver PROPERTIES
		COMPILE_DEFINITIONS _OREGON_REALM_CONFIG="${CONF_DIR}/oregonrealm.conf"
)
endif()

if( UNIX )
  set(authserver_LINK_FLAGS "-pthread ${authserver_LINK_FLAGS}")
endif()

if( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
  set(authserver_LINK_FLAGS "-framework Carbon ${authserver_LINK_FLAGS}")
endif()

set_target_properties(authserver PROPERTIES LINK_FLAGS "${authserver_LINK_FLAGS}")

if( WIN32 )
  target_link_libraries(
    authserver
    shared
    ${MYSQL_LIBRARY}
    ${OPENSSL_LIBRARIES}
    ${OPENSSL_EXTRA_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
  )
else()
  target_link_libraries(
    authserver
    shared
    ${MYSQL_LIBRARY}
    ${OPENSSL_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
    ${OSX_LIBS}
  )
endif()

if( WIN32 )
  add_custom_command(TARGET authserver
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/oregonrealm.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
  )
endif()

if( UNIX )
  install(TARGETS authserver DESTINATION bin)
  install(FILES oregonrealm.conf.dist DESTINATION ${CONF_DIR})
elseif( WIN32 )
  install(TARGETS authserver DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES oregonrealm.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

# Generate precompiled header
if ( USE_COREPCH )
	if ( CMAKE_COMPILER_IS_GNUCXX )
		add_precompiled_header(authserver ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders/authPCH.h)
	elseif( MSVC )
		add_native_precompiled_header(authserver ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders/authPCH)
	endif()
endif()