macro(MoveConfigFiles mSources mDest mDelSource)
	if(NOT DEFINED mDelSource)
		set(mDelSource false)
	endif(NOT DEFINED mDelSource)
	
	message(STATUS "++ Copy files to ${mDest}")
	
	if("${mDest}" MATCHES "[/]$")
		set(mSep "")
	else("${mDest}" MATCHES "[/]$")
		set(mSep "/")
	endif("${mDest}" MATCHES "[/]$")

	FOREACH (current_FILE ${mSources})
		get_filename_component(MOVE_NAME ${current_FILE} NAME)
		FILE(READ ${current_FILE} TEMP_READ)
		FILE(WRITE "${mDest}${mSep}${MOVE_NAME}" "${TEMP_READ}")
			
		if(mDelSource)
			file(REMOVE ${current_FILE})
		endif(mDelSource)
	ENDFOREACH (current_FILE)

endmacro(MoveConfigFiles)
