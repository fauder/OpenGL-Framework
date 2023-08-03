#pragma once

// Project Includes.
#include "../Test/Test.hpp"

// std Includes.
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace Framework::Test
{
	/* Displays a list of tests for the user to choose from. Upon user selection, stops execution of the menu and creates the selected test on the heap & updates its unique_ptr. */
	class Test_Menu : public Test< Test_Menu >
	{
		friend Test< Test_Menu >;
		using MapType = std::map< std::string, std::function< std::unique_ptr< TestInterface >() > >;

	public:
		Test_Menu( Renderer& renderer, std::unique_ptr< TestInterface >& current_test );

		template< class TestClass, typename... ConstructorParams >
		void Register( ConstructorParams... params )
		{
			const auto name = ExtractTestNameFromTypeName( typeid( TestClass ).name() );
			test_creation_info_by_name[ name ] = [ & ]() { return std::make_unique< TestClass >( renderer, std::forward< ConstructorParams >( params )... ); };
		}

		template< class TestClass, typename... ConstructorParams >
		void Register( const std::string& test_name, ConstructorParams... params )
		{
			test_creation_info_by_name[ test_name ] = [ & ]() { return std::make_unique< TestClass >( renderer, params... ); };
		}

		void Unregister( const std::string& name );

	protected:
		void OnRenderImGui();

	private:
		std::unique_ptr< TestInterface >& test_current;
		MapType test_creation_info_by_name;
	};
}
