
#include <q-io/ip.hpp>

#include "q-io-test.hpp"

#include <vector>

TEST( ip, ipv4 )
{
	q::io::ipv4_address ipv4_lo( "127.0.0.1" );
	q::io::ipv4_address ipv4_goo( "8.8.8.8" );

	EXPECT_TRUE( ipv4_lo.valid );
	EXPECT_EQ( ipv4_lo.string( ), "127.0.0.1" );
	EXPECT_TRUE( ipv4_goo.valid );
	EXPECT_EQ( ipv4_goo.string( ), "8.8.8.8" );
}

TEST( ip, ipv4_invalid )
{
	std::vector< std::string > invalids{
		"",
		"0",
		"1",
		"127.0.0.0.0",
		"127.0.256.1",
		"::1"
	};

	for ( auto& invalid : invalids )
	{
		auto addr = q::io::ipv4_address::from( invalid );
		EXPECT_FALSE( addr.valid );
	}
}

TEST( ip, ipv6 )
{
	q::io::ipv6_address ipv6_lo( "::1" );
	q::io::ipv6_address ipv6_goo( "2001:4860:4860::8888" );
	q::io::ipv6_address ipv6_zero( "::" );

	EXPECT_TRUE( ipv6_lo.valid );
	EXPECT_EQ( ipv6_lo.string( ), "::1" );
	EXPECT_TRUE( ipv6_goo.valid );
	EXPECT_EQ( ipv6_goo.string( ), "2001:4860:4860::8888" );
	EXPECT_TRUE( ipv6_zero.valid );
	EXPECT_EQ( ipv6_zero.string( ), "::" );
}

TEST( ip, ipv6_invalid )
{
	std::vector< std::string > invalids{
		"",
		"0",
		"1",
		"12345::1",
		"::-1",
		"1::2::3",
		"1:2:3:4:5:6:7",
		"1:2:3:4:5:6:7:8:9"
	};

	for ( auto& invalid : invalids )
	{
		auto addr = q::io::ipv6_address::from( invalid );
		EXPECT_FALSE( addr.valid );
	}
}