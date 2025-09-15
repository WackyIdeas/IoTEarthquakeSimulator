project(SSDP_CPP)
add_library(lssdpcpp STATIC
            lssdpcpp/lssdpcpp.h
            lssdpcpp/lssdpcpp.cpp

            url/url.hpp
            url/url.cpp)
