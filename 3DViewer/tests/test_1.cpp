#include "tests_entry.h"

TEST(CONFIG, TEST_1) {
  Config config;
  EXPECT_EQ(config.BgColor(), default_bg_color);
}

// Example test case
TEST(ExampleTest, BasicAssertions) {
  EXPECT_EQ(1 + 1, 2);
  EXPECT_TRUE(true);
}

TEST(COMMANDS, ParseObj) {
  SceneModel mock_model;
  mock_model.SetFilePath(SAMPLES_DIR "/models/cube.obj");
  ParseOBJFileCommand command(&mock_model);
  EXPECT_TRUE(command.execute());
  const auto& raw_data = mock_model.getRawData();
  EXPECT_EQ(raw_data.CountVertices(), 8);
  EXPECT_EQ(raw_data.CountFaces(), 12);
  EXPECT_EQ(raw_data.GetVertex(6), QVector3D(-0.25, -0.25, -0.25));
  EXPECT_EQ(raw_data.GetNormal(1), QVector3D(0, 1, 0));

  // EXPECT_TRUE(true);
}

TEST(COMMANDS, ParseObj2) {
  SceneModel mock_model;
  mock_model.SetFilePath(SAMPLES_DIR "/models/Trump.obj");
  ParseOBJFileCommand command(&mock_model);
  EXPECT_TRUE(command.execute());
  const auto& raw_data = mock_model.getRawData();
  EXPECT_EQ(raw_data.CountVertices(), 639);
  EXPECT_EQ(raw_data.CountFaces(), 1270);
  EXPECT_EQ(raw_data.CountFaces(), 1270);
  EXPECT_EQ(raw_data.MaterialsLib(), "Trump.mtl");
  EXPECT_FALSE(raw_data.TextureCoords().empty());
  EXPECT_EQ(raw_data.GetTextureCoords(0), QVector2D(0.530141, 0.652631));
}