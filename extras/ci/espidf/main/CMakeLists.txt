idf_component_register(SRCS "main.cpp"
                       INCLUDE_DIRS "")
                       
# required to break possible dependency cycle                       
set_property(TARGET ${COMPONENT_LIB} APPEND PROPERTY LINK_INTERFACE_MULTIPLICITY 3)
