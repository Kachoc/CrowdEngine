#include <iostream>
#include <sstream>
#include <Windows.h>

#include <Engine.h>

#include <Mesh.h>

using namespace Crowd;
using namespace std;

#define NUM 1
#define NUM_SHADOW_SCENES 3
#define NUM_SHADOW_MAPS 4

void CalculateCascadedShadowMapsSize(const PerspectiveCamera &view, std::vector<LightScene::SunShadowMapLevel> &size);

void CalculateCascadedShadowMapsCameras(const PerspectiveCamera &view, float sunAngleX, float sunAngleY, float depthOfField, const std::vector<float> &mapsSize, std::vector<OrthographicCamera> &cameras)
{
	unsigned int numLevels = cameras.size();

	PerspectiveCamera::Parameters params;
	view.GetParameters(params);

	//
	//	Calcul des tailles des différentes maps
	//
	float frustumUnitHeight = tan(params.fovY);
	float frustumUnitWidth = frustumUnitHeight*params.aspectRatio;
	float frustumUnitRadius = sqrt(frustumUnitHeight*frustumUnitHeight + frustumUnitWidth*frustumUnitWidth);

	//
	//	Calcul des positions et orientations des différentes maps
	//
	Vector3 eye = view.GetPosition();
	Matrix viewMatrix = view.GetView();
	Vector3 lookDir(viewMatrix._13, viewMatrix._23, viewMatrix._33);
	lookDir.Normalize();

	float degToRad = 1.0f/180.0f*(float)CRWD_PI;
	Vector3 SunDirection = Vector3(cos(sunAngleX*degToRad)*cos(sunAngleY*degToRad), sin(sunAngleX*degToRad), cos(sunAngleX*degToRad)*sin(sunAngleY*degToRad));
	for(unsigned int i = 0; i < numLevels; i++)
	{
		float depth = depthOfField + 0.5f*depthOfField*mapsSize[i]/mapsSize[numLevels-1];
		cameras[i].SetParameters(mapsSize[i], mapsSize[i], depth);

		float zCenterPosition = mapsSize[i]/frustumUnitRadius;

		Vector3 cameraPosition = eye - SunDirection*depthOfField*0.5f + lookDir*zCenterPosition/2.0f;
		cameras[i].SetOrientation(cameraPosition, sunAngleX, sunAngleY-90.0f, 0);
	}
}

#undef min
#undef max

int main( HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd )
{
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	try
	{
		Status status;

		String wndCaption("Application test");
		Window mainWnd(wndCaption, hInstance);

		Renderer &renderer = Renderer::GetInstance();
		renderer.SetTargetWindowForDifRender(mainWnd.GetWindow(), FORMAT_R8G8B8A8_UNORM, FORMAT_R16G16B16A16_FLOAT, FORMAT_R16G16B16A16_SNORM);

		PostProcess &postProManager = PostProcess::GetInstance();

		InputManager &input = InputManager::GetInstance();
		input.Initialize(mainWnd);

		ShaderLibrary &shaders = ShaderLibrary::GetInstance();
		ShaderLibrary::SetShadersPath("..\\CrowdEngine\\");
		status = shaders.LoadStandardShaders();
		if(!status) {
			throw Exception(status.asString());
		}

		Vector3 minPoint(-1000.0f, -1000.0f, -1000.0f);
		Vector3 maxPoint(1000.0f, 1000.0f, 1000.0f);
		AABB aabb(minPoint, maxPoint);
		GeometryScene geoScene(aabb);

		//
		//	Font
		//
		Font::ptr font = Font::Create(14, 0, false, false, "Times New Roman");

		//
		//	Texture
		//
		Texture::ptr whiteTex = Texture::Load("..\\Textures\\White.png");
		Texture::ptr terrainDefaultTex = Texture::Load("..\\Textures\\TerrainDefault.jpg");
		Texture::ptr mountainTex = Texture::Load("..\\Textures\\TerrainDefault_large.jpg");
		Texture::Save("..\\Textures\\TerrainDefault_test_save.png", terrainDefaultTex, "png");

		// DL Pass
		SunPass::ptr DLPass = SunPass::Create();

		// LightScene
		AABB lightSceneAABB = aabb;
		LightScene::ptr lightScene = LightScene::Create(lightSceneAABB);
		Vector3 SunDirection(-0.0f, -4.0f, 12.0f);
		lightScene->SetSunDirection(SunDirection);
		lightScene->SetSunDiffuse(Color(1.0f, 1.0f, 1.0f, 1.0f)*1.2f);
		lightScene->SetSunSpecular(Color(1.0f, 1.0f, 1.0f, 1.0f)*1.2f);
		lightScene->SetSunAmbient(Color(0.05f, 0.05f, 0.05f, 1.0f));

		Color		SkyColor = Color(73, 130, 189, 255)/255.0f;
		Color		HorizonColor = Color(114, 154, 196, 255)/255.0f*1.2f;
		float		SkyBlendExponent = 11.0f;
		float		SunRadiusExponent = 192.0f;
		float		SunRadiusMultiplier = 0.65f;

		OrthographicCamera sunCamera;
		sunCamera.SetOrientation(SunDirection*(-5.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
		sunCamera.SetParameters(16.0f, 16.0f, 100.0f);

		aabb.MinPoint = Vector3(-1000.0f, -1000.0f, -1000.0f);
		aabb.MaxPoint = Vector3(1000.0f, 1000.0f, 1000.0f);
		lightScene->InitializeShadowScenes(aabb, NUM_SHADOW_SCENES);

		//
		//	PointLights
		//
		PointLight::ptr lights[3];
		lights[0] = PointLight::Create(Vector4(3.0f, 2.0f, 3.0f, 1.0f), Color(2.0f, 0, 0, 1.0f));
		lights[1] = PointLight::Create(Vector4(3.0f, 4.0f, -3.0f, 1.0f), Color(0, 2.0f, 0, 1.0f));
		lights[2] = PointLight::Create(Vector4(-3.0f, 2.0f, 3.0f, 1.0f), Color(0, 0, 2.0f, 1.0f));

		lights[0]->UseShadowMaps(1024, 0.01f, 1000.0f, 0, true);
		lights[1]->UseShadowMaps(1024, 0.05f, 100.0f, 0, true);
		lights[2]->UseShadowMaps(1024, 0.05f, 100.0f, 0, true);

		lightScene->Add(lights[0]);
		lightScene->Add(lights[1]);
		lightScene->Add(lights[2]);

		//
		//	Mountain
		//
		Mesh::ptr mountain = Mesh::Load("..\\Meshes\\Mountains7R2.cpm", CPM_LOAD_POSITION || CPM_LOAD_NORMAL || CPM_LOAD_UV, &status);
		if(!status) {
			throw Exception(status.asString());
		}
		Color mntColor = Color(0.5f, 0.5f, 0.5f, 230.0f/255.0f);
		LambertShader::ptr mountainShader = LambertShader::Create(mntColor, whiteTex);
		SGEntity::ptr mountainSG = SGEntity::Create(mountain, mountainShader, mountain->GetAABB());
		geoScene.Add(mountainSG);

		Mesh::ptr mountainShadow = Mesh::Load("..\\Meshes\\Mountains7R2.cpm", CPM_LOAD_POSITION, &status);
		if(!status) {
			throw Exception(status.asString());
		}
		DepthShader::ptr mountainDepthShader = DepthShader::Create();
		SGEntity::ptr mountainDepth = SGEntity::Create(mountainShadow, mountainDepthShader, mountainShadow->GetAABB());

		lightScene->AddShadowGeometry(mountainDepth, 0);


		//
		//	Plane
		//
		Matrix planePosition(Matrix::Identity());
		planePosition._42 = -1.6f;
		planePosition._11 = 100.0f;
		planePosition._22 = 0.05f;
		planePosition._33 = 100.0f;

		Mesh::ptr plane = Mesh::Load("..\\Meshes\\Cube.cpm", CPM_LOAD_POSITION || CPM_LOAD_NORMAL || CPM_LOAD_UV, &status);
		if(!status) {
			throw Exception(status.asString());
		}
		Color planeColor = Color(0.5f, 0.5f, 0.5f, 230.0f/255.0f);
		LambertShader::ptr planeShader = LambertShader::Create(planeColor, whiteTex);
		SGEntity::ptr planeSG = SGEntity::Create(plane, planeShader, plane->GetAABB());
		geoScene.Add(planeSG);
		planeSG->SetPositionMatrix(planePosition);

		Mesh::ptr planeShadow = Mesh::Load("..\\Meshes\\Cube.cpm", CPM_LOAD_POSITION, &status);
		if(!status) {
			throw Exception(status.asString());
		}
		DepthShader::ptr planeDepthShader = DepthShader::Create();
		SGEntity::ptr planeDepth = SGEntity::Create(planeShadow, planeDepthShader, planeShadow->GetAABB());

		lightScene->AddShadowGeometry(planeDepth, 0);
		planeDepth->SetPositionMatrix(planePosition);

		//
		//	Field
		//
		/*Texture::ptr heightMap = Texture::Load("..\\Textures\\Perlin_noise_1.jpg");

		Field::ptr field = Field::Create(1000);// rManager.Create<Field>("Field", &fieldInfo);

		LambertShader::ptr fieldShader = LambertShader::Create(Color(0.5f, 0.5f, 0.5f, 100.0f/255.0f), terrainDefaultTex); //rManager.Create<LambertShader>("LambertShader", NULL);
		DepthShader::ptr fieldDepthShader = DepthShader::Create(); //rManager.Create<DepthShader>("DepthShader", NULL);

		TerrainShader::Parameters terrainParams;
		terrainParams.Diffuse = Color(0.5f, 0.5f, 0.5f, 100.0f/255.0f);
		terrainParams.HeightConstant = -2.0f;
		terrainParams.HeightMultiplier = 4.0f;
		terrainParams.SquareWidth = 0.02f;
		terrainParams.SquareUVWidth = 0.001f;
		terrainParams.TerrainWidth = 100;
		terrainParams.HeightMap = heightMap;
		terrainParams.DiffuseSpecularTexture = mountainTex;
		TerrainShader::ptr terrainShader = TerrainShader::Create(terrainParams); //rManager.Create<TerrainShader>("TerrainShader", NULL);

		SGEntity::ptr fieldSG;
		SGEntity::ptr shadowField;
		Matrix position(Matrix::Identity());

		fieldSG = SGEntity::Create(field, terrainShader, field->GetAABB());
		fieldSG->SetPosition(position);
		shadowField = SGEntity::Create(field, fieldDepthShader, field->GetAABB());

		geoScene.Add(fieldSG);
		lightScene->AddShadowGeometry(shadowField, 0);*/

		//
		//	Ajout des objets dans les shadow scenes
		//
		unsigned int shadowMapLinking[] = { 0, 0, 0, 0 };
		std::vector<float> sunShadowMapsSizes;//
		sunShadowMapsSizes.resize(NUM_SHADOW_MAPS);//
		sunShadowMapsSizes[0] = 2.0f;
		sunShadowMapsSizes[1] = 5.0f;
		sunShadowMapsSizes[2] = 12.0f;
		sunShadowMapsSizes[3] = 30.0f;
		std::vector<float> shadowMapEpsilons;
		shadowMapEpsilons.resize(4);
		shadowMapEpsilons[0] = 0.001f;
		shadowMapEpsilons[1] = 0.01f;
		shadowMapEpsilons[2] = 0.02f;
		shadowMapEpsilons[3] = 0.03f;
		lightScene->CreateSunShadowMaps(2048, NUM_SHADOW_MAPS, shadowMapEpsilons, shadowMapLinking);


		// Camera
		RenderInfo renderInfo;
		RECT rect;
		mainWnd.GetClientRectangle(rect);

		float radius = 6.0f;
		float pulsation = 0.08f;
		float speed = 3.0f;
		float angle = 0.0f;

		float angleX = -20.0f;
		float angleY = -60.0f;

		float sunAngleX = -45.0f;
		float sunAngleY = 90.0f;
		float sunRadius = 50.0f;
		float sunPulsation = 0.08f;

		Vector3	//eye(cos(angle) * radius, 2.0f, sin(angle) * radius),
					eye(-12.3f, 4.2f, -5.7f),
					lookAt(0.0f, 2.0f, 0.0f),
					up(0.0f, 1.0f, 0.0f);

		float fovY = (float) CRWD_PI * 0.25f;
		float aspectRatio = (float) rect.right / (float) rect.bottom;
		float zn = 0.1f, zf = 1000.0f;

		PerspectiveCamera camera;
		camera.SetParameters(fovY, aspectRatio, zn, zf);
		camera.SetOrientation(eye, lookAt, up);
		
		//
		//	Boucle des messages
		//
		Timer<double> timer;

		double timeTot = 0;
		double elapsedTime;
		double timePerFrame = 0;
		unsigned int  numFrames = 0;
		double fFrames = 0;

		unsigned int numShadowVertices = 0;
		unsigned int numShadowTris = 0;

		MSG msg = {0};
		bool quit = false;
		while(!quit)
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if(msg.message == WM_QUIT)
				{
					quit = true;
					break;
				}
			}

			if(!quit)
			{
				renderer.GetRenderInfo(renderInfo);

				camera.SetOrientation(eye, angleX, angleY, 0.0f);

				Matrix m;
				float degToRad = 1.0f/180.0f*(float)CRWD_PI;
				SunDirection = Vector3(cos(sunAngleX*degToRad)*cos(sunAngleY*degToRad), sin(sunAngleX*degToRad), cos(sunAngleX*degToRad)*sin(sunAngleY*degToRad));
				sunCamera.SetOrientation(SunDirection*(-sunRadius), sunAngleX, sunAngleY-90.0f, 0);
				lightScene->SetSunDirection(SunDirection);

				WorldShaderInfo worldShaderInfo;
				PerspectiveCamera::Parameters cameraParams;
				camera.GetParameters(cameraParams);

				ZeroMemory(&worldShaderInfo, sizeof(WorldShaderInfo));
				Matrix::Transpose(worldShaderInfo.projMatrix, camera.GetProjection());
				Matrix::Transpose(worldShaderInfo.viewMatrix, camera.GetView());
				Matrix::Transpose(worldShaderInfo.viewProjMatrix, camera.GetViewProj());
				Matrix::Invert(m, camera.GetViewProj());
				Matrix::Transpose(worldShaderInfo.invViewProjMatrix, m);
				Matrix::Invert(m, camera.GetView());
				Matrix::Transpose(worldShaderInfo.invViewMatrix, m);

				worldShaderInfo.cameraPosition = Vector4(eye, 1.0f);
				Matrix view = camera.GetView();
				worldShaderInfo.cameraLookDir = Vector4(view._13, view._23, view._33, 0.0f);
				worldShaderInfo.cameraLookDir.Normalize();

				worldShaderInfo.cameraFovY = cameraParams.fovY;
				worldShaderInfo.cameraRatio = cameraParams.aspectRatio;
				worldShaderInfo.cameraNearZ = cameraParams.nearZ;
				worldShaderInfo.cameraFarZ = cameraParams.farZ;
				shaders.SetWorldShaderInfo(worldShaderInfo);
				lightScene->SetWorldShaderInfo(worldShaderInfo);


				// Rendu des shadow maps
				renderer.InitDrawnGeometryCount();

				std::vector<OrthographicCamera> sunCameras;
				sunCameras.resize(NUM_SHADOW_MAPS);

				CalculateCascadedShadowMapsCameras(camera, sunAngleX, sunAngleY, 128.0f, sunShadowMapsSizes, sunCameras);

				lightScene->RenderSunShadowMaps(renderInfo, sunCameras);

				lightScene->ComputeShadowMaps(renderInfo, camera);

				numShadowVertices = renderer.GetDrawnVerticesCount();
				numShadowTris = renderer.GetDrawnTrisCount();
				
				// Pré-rendu de la géométrie
				renderer.InitDrawnGeometryCount();
				renderer.BeginGeometryPass();
				renderer.RenderScene();

				geoScene.Draw(renderInfo, camera);

				// Rendu des lumières
				renderer.BeginLightPass();
				lightScene->SetSkyDesc(SkyColor, HorizonColor, SkyBlendExponent, SunRadiusExponent, SunRadiusMultiplier);
				lightScene->RenderSun(renderInfo, camera);

				lightScene->ApplyLightPass(renderInfo, camera);

				// Post-processing
				renderer.BeginPostPass();

				PostProcess::Parameters ppManagerParams;
				ppManagerParams.FogColor = Color(197, 226, 236, 0.0f)/255.0f;
				ppManagerParams.FogHeightDim = 2.0f;
				ppManagerParams.FogIntensity = 0.3f;
				ColorRGB averageColor = postProManager.GetAverageColor();
				float colorIntensity = min(max(max3(averageColor.r, averageColor.g, averageColor.b), 0.06f), 3.0f);
				ppManagerParams.ColorIntensityAdjustment = 1.0f; //(float) pow((double) (0.3f/colorIntensity), 0.25);				
				postProManager.SetParameters(ppManagerParams);

				postProManager.PerformPostProcessRender(renderInfo);

				// Affichage du texte
				renderer.BeginPostPass(); ////////// Provisoire (la fonction ne devrait pas être rappelée être ici)
				std::ostringstream oss;
				oss << fFrames << " FPS\n";
				oss << "time per frame: " << 1000*timePerFrame << " ms\n";
				oss << numShadowVertices << " shadow vertices\n";
				oss << numShadowTris << " shadow triangles\n";
				oss << renderer.GetDrawnVerticesCount() << " vertices\n";
				oss << renderer.GetDrawnTrisCount() << " triangles\n";
				String fpsStr = oss.str();
				RECT strPosition;
				strPosition.top = 10;
				strPosition.left = 10;
				strPosition.bottom = strPosition.top + 100;
				strPosition.right = strPosition.left + 300;
				font->Write(fpsStr, strPosition, 0, Color(1.0f, 0.0f, 0.0f, 1.0f));

				// Fin du rendu
				renderer.Present(false);
			}

			//
			//	Calcul du framerate
			//

			elapsedTime = timer.GetElapsedTime();
			timeTot += elapsedTime;
			input.Update();

			numFrames++;
			if(timeTot >= 0.5)
			{
				fFrames = (double) numFrames;
				fFrames /= timeTot;
				timePerFrame = timeTot / (double) numFrames;
				numFrames = 0;
				timeTot = 0;

				std::ostringstream oss;
				oss << wndCaption << ": " << fFrames << " FPS.";
				std::string caption = oss.str();
				mainWnd.SetCaption(caption.c_str());
			}

			//
			//	Déplacement de la caméra
			//
			MouseState mouseState = input.GetMouse();
			if(mouseState.buttons[1])
			{
				angleY -= (float) mouseState.transX*pulsation;
				angleX -= (float) mouseState.transY*pulsation;
				int numTours;
				numTours = (int) angleX/360;
				angleX = angleX-360*(float)numTours;
				numTours = (int) angleY/360;
				angleY = angleY-360*(float)numTours;
			}
			else if(mouseState.buttons[2])
			{
				sunAngleY -= (float) mouseState.transX*sunPulsation;
				sunAngleX -= (float) mouseState.transY*sunPulsation;
				int numTours;
				numTours = (int) sunAngleX/360;
				sunAngleX = sunAngleX-360*(float)numTours;
				numTours = (int) sunAngleY/360;
				sunAngleY = sunAngleY-360*(float)numTours;
			}

			const char *keyboard = input.GetKeyboard();

			Matrix view = camera.GetView();
			Vector3 lookDir(view._13, view._23, view._33);
			Vector3 lookUp(view._12, view._22, view._32);
			Vector3 lookRight(view._11, view._21, view._31);
			lookDir.Normalize();
			lookUp.Normalize();
			lookRight.Normalize();
			if(keyboard[17]) { eye = eye+lookDir*speed*(float)elapsedTime; }
			if(keyboard[31]) { eye = eye-lookDir*speed*(float)elapsedTime; }
			if(keyboard[30]) { eye = eye-lookRight*speed*(float)elapsedTime; }
			if(keyboard[32]) { eye = eye+lookRight*speed*(float)elapsedTime; }
			if(keyboard[16]) { eye = eye-Vector3(0.0f, 1.0f, 0.0f)*speed*(float)elapsedTime; }
			if(keyboard[18]) { eye = eye+Vector3(0.0f, 1.0f, 0.0f)*speed*(float)elapsedTime; }
			if(keyboard[CRWD_KBD_UP]) { SunRadiusExponent += 36.0f*(float)elapsedTime; }
			if(keyboard[CRWD_KBD_DOWN]) { SunRadiusExponent -= 36.0f*(float)elapsedTime; }
			if(keyboard[CRWD_KBD_LEFT]) { SunRadiusMultiplier += 2.0f*(float)elapsedTime; }
			if(keyboard[CRWD_KBD_RIGHT]) { SunRadiusMultiplier -= 2.0f*(float)elapsedTime; }
			if(keyboard[CRWD_KBD_P]) { SkyBlendExponent += 36.0f*(float)elapsedTime; }
			if(keyboard[CRWD_KBD_M]) { SkyBlendExponent -= 36.0f*(float)elapsedTime; }
			
			for(unsigned int i = 0; i < 255; i++)
			{
				if(keyboard[i])
				{
					int j = 0;
				}
			}
		}
	}
	catch(Exception &e)
	{
		Logger::Error(e.what());
	}

	return 0;
}