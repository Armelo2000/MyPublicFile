/*
 * tests.cpp
 *
 *  Created on: 02.12.2022
 *      Author: melah
 */

#include <gtest/gtest.h>
#include "Spi/SpiCore.h"

#if (UINTMAX_MAX > UINT32_MAX)
#error "This test use a x64 version. But the controller is 32 bits";
#endif

TEST(MyFirstTestClass, NotInitializedSpi)
{
	SpiCore spi;
	EXPECT_EQ(nullptr, spi.getSpiHandle(1));

}

TEST(MyFirstTestClass, InitializedSpi)
{
	SpiCore spi;
	spi.SpiInit(1);
	EXPECT_NE(nullptr, spi.getSpiHandle(1));

}

TEST(MyFirstTestClass, ThisTestFails)
{
  FAIL();
}

struct TTestFixtureSpi: testing::Test{
	SpiCore *m_spi;
	TTestFixtureSpi(){
		m_spi = new SpiCore();
	}

	~TTestFixtureSpi(){
		delete m_spi;
	}
};

TEST_F(TTestFixtureSpi, TestBeforeInitialization)
{
  EXPECT_EQ(m_spi->getSpiHandle(1), nullptr);
}

TEST_F(TTestFixtureSpi, TestAfterInitialization)
{
  m_spi->SpiInit(1);
  EXPECT_NE(m_spi->getSpiHandle(1), nullptr);
}
struct TSpiID{
	bool init;
	uint16_t id;
	bool isHandlerNotNull;
};
struct TTestSpiIndex: TTestFixtureSpi, testing::WithParamInterface<TSpiID>{
	TTestSpiIndex(){
		if(GetParam().init)
			m_spi->SpiInit(GetParam().id);
	}
};

TEST_P(TTestSpiIndex, spiIndex){
	auto spi = GetParam();
	auto spiHandle = m_spi->getSpiHandle(spi.id);
	auto isNotNull = (spiHandle == nullptr)?false:true;
	EXPECT_NE(isNotNull, spi.isHandlerNotNull);
}

INSTANTIATE_TEST_CASE_P(Default, TTestSpiIndex,
		testing::Values(
				TSpiID{false, 3, false},
				TSpiID{false, 5, true},
				TSpiID{true, 2, false},
				TSpiID{true, 4, true}

));

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from %s\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  auto x = UINTMAX_MAX; UINTMAX_MAX;

  return RUN_ALL_TESTS();
}
