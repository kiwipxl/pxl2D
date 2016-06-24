#ifndef _CONFIG_H
#define _CONFIG_H

namespace pxl { namespace system {
    
    #include <string>

    /** -------------------------------------------------------
					    PXL config defines
    ------------------------------------------------------- **/

    //exception config
    #define CONFIG_SHOW_EXCEPTIONS_IN_DEBUG			1			/**< Defines whether exceptions will show in debug mode or not **/
    #define CONFIG_SHOW_EXCEPTIONS_IN_RELEASE		0			/**< Defines whether exceptions will show in release mode or not **/
    #define CONFIG_EXCEPTION_TITLE					"Error"		/**< The error title used when logged **/
    #define CONFIG_SHOW_STACK_TRACE					1			/**< Defines whether or not to show stack traces on exception messages **/

    //batch config
    #define CONFIG_BATCH_VERTEX_RESIZE                 64           /**< Incremental vertex batch resize - the amount to resize and allocate if a new add goes over the vertex batch capacity vector **/
    #define CONFIG_BATCH_INDICES_RESIZE                48           /**< Incremental indices batch resize - the amount to resize and allocate if a new add goes over the indices capacity vector **/

    /** -------------------------------------------------------
					    PXL error codes
	    ------------------------------------------------------- **/

    typedef std::string ErrorCode;

    #define ERROR_UNKNOWN                           "UNKNOWN"
    #define ERROR_SWAP_BUFFERS_FAILED               "SWAP_BUFFERS_FAILED"
    #define ERROR_MISSING_SHADER_HEADERS            "MISSING_SHADER_HEADERS"
    #define ERROR_INVALID_FILE                      "INVALID_FILE"
    #define ERROR_EMPTY_FILE                        "EMPTY_FILE"
    #define ERROR_SHADER_LINK_FAILED                "SHADER_LINK_FAILED"
    #define ERROR_SHADER_COMPILE_FAILED             "SHADER_COMPILE_FAILED"
    #define ERROR_INVALID_PNG                       "INVALID_PNG"
    #define ERROR_TEXTURE_CREATION_FAILED           "TEXTURE_CREATION_FAILED"
    #define ERROR_BATCH_ADD_FAILED                  "BATCH_ADD_FAILED"
    #define ERROR_TEXTURE_SHEET_CREATION_FAILED     "TEXTURE_SHEET_CREATION_FAILED"
    #define ERROR_TEXTURE_SHEET_ADD_FAILED          "TEXTURE_SHEET_ADD_FAILED"
}};

#endif