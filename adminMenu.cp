using System;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

// Token: 0x0200001F RID: 31
public class AdminMenu : MonoBehaviour
{
	// Token: 0x060000A5 RID: 165 RVA: 0x00005BF8 File Offset: 0x00003DF8
	private void Awake()
	{
		if (AdminMenu.instance != null && AdminMenu.instance != this)
		{
			UnityEngine.Object.Destroy(base.gameObject);
			return;
		}
		AdminMenu.instance = this;
		UnityEngine.Object.DontDestroyOnLoad(base.gameObject);
		this.CreateInGameCanvas();
		this.CreateHudCanvas();
		Debug.Log("<color=red>Admin Console</color> 1.0.0 by wittylunar loaded. Press P.");
	}

	// Token: 0x060000A6 RID: 166 RVA: 0x00005C54 File Offset: 0x00003E54
	private Texture2D MakeTex(int w, int h, Color c)
	{
		Color[] array = new Color[w * h];
		for (int i = 0; i < array.Length; i++)
		{
			array[i] = c;
		}
		Texture2D texture2D = new Texture2D(w, h);
		texture2D.SetPixels(array);
		texture2D.Apply();
		return texture2D;
	}

	// Token: 0x060000A7 RID: 167 RVA: 0x00005C94 File Offset: 0x00003E94
	private void Update()
	{
		if (Input.GetKeyDown(KeyCode.P))
		{
			AdminMenu.menuOpen = !AdminMenu.menuOpen;
			if (AdminMenu.menuOpen)
			{
				Cursor.lockState = CursorLockMode.None;
				Cursor.visible = true;
			}
			else
			{
				Cursor.lockState = CursorLockMode.Locked;
				Cursor.visible = false;
			}
		}
		if (Input.GetKeyDown(KeyCode.Z))
		{
			AdminMenu.flyEnabled = !AdminMenu.flyEnabled;
		}
		if (Input.GetKeyDown(KeyCode.F2))
		{
			AdminMenu.godMode = !AdminMenu.godMode;
		}
		if (Input.GetKeyDown(KeyCode.X))
		{
			AdminMenu.noclip = !AdminMenu.noclip;
		}
		if (Input.GetKeyDown(KeyCode.E))
		{
			AdminMenu.espEnabled = !AdminMenu.espEnabled;
		}
		if (Input.GetKeyDown(KeyCode.T))
		{
			AdminMenu.tracersEnabled = !AdminMenu.tracersEnabled;
		}
		if (AdminMenu.godMode)
		{
			this.ApplyGodMode();
		}
		if (AdminMenu.noclip)
		{
			this.ApplyNoclip();
		}
		if (Time.unscaledTime - this.lastEnemyCacheTime > 0.5f)
		{
			this.lastEnemyCacheTime = Time.unscaledTime;
			this.CacheEnemies();
		}
		if (Time.unscaledTime - this.lastSceneCheckTime > 0.3f)
		{
			this.lastSceneCheckTime = Time.unscaledTime;
			this.UpdateHudText();
		}
		if (Input.GetKey(KeyCode.B))
		{
			this.spawnTimer += Time.unscaledDeltaTime;
			if (this.spawnTimer >= this.spawnInterval)
			{
				this.spawnTimer = 0f;
				this.SpawnOneEnemy();
			}
		}
		else
		{
			this.spawnTimer = 0f;
		}
		if (Input.GetKeyDown(KeyCode.N))
		{
			this.SpawnOneEnemy();
		}
	}

	// Token: 0x060000A8 RID: 168 RVA: 0x00005E04 File Offset: 0x00004004
	private static void SetFloat(object o, string name, float v)
	{
		if (o == null)
		{
			return;
		}
		FieldInfo field = o.GetType().GetField(name, BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
		if (field != null && field.FieldType == typeof(float))
		{
			field.SetValue(o, v);
		}
	}

	// Token: 0x060000A9 RID: 169 RVA: 0x00005E54 File Offset: 0x00004054
	private static void SetBool(object o, string name, bool v)
	{
		if (o == null)
		{
			return;
		}
		FieldInfo field = o.GetType().GetField(name, BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
		if (field != null && field.FieldType == typeof(bool))
		{
			field.SetValue(o, v);
		}
	}

	// Token: 0x060000AA RID: 170 RVA: 0x00005EA4 File Offset: 0x000040A4
	private void ApplyGodMode()
	{
		try
		{
			foreach (MonoBehaviour monoBehaviour in UnityEngine.Object.FindObjectsOfType<MonoBehaviour>())
			{
				if (!(monoBehaviour == null))
				{
					FieldInfo field = monoBehaviour.GetType().GetField("health", BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
					if (field != null && field.FieldType == typeof(float))
					{
						field.SetValue(monoBehaviour, 999999f);
					}
					else if (field != null && field.FieldType == typeof(int))
					{
						field.SetValue(monoBehaviour, 999999);
					}
					if (monoBehaviour.GetType().Name == "Enemy")
					{
						Collider component = monoBehaviour.GetComponent<Collider>();
						if (component != null)
						{
							component.enabled = false;
						}
					}
				}
			}
		}
		catch
		{
		}
	}

	// Token: 0x060000AB RID: 171 RVA: 0x00005F98 File Offset: 0x00004198
	private void OnGUI()
	{
		this.EnsureStyles();
		this.DrawESP();
		if (!AdminMenu.menuOpen)
		{
			return;
		}
		GUI.DrawTexture(new Rect(0f, 0f, (float)Screen.width, (float)Screen.height), this.bgTex);
		GUILayout.BeginArea(new Rect((float)(Screen.width / 2 - 300), 10f, 600f, 50f), this.boxStyle);
		GUILayout.BeginHorizontal(Array.Empty<GUILayoutOption>());
		GUILayout.FlexibleSpace();
		GUILayout.Label("ADMIN CONSOLE", this.titleStyle, new GUILayoutOption[]
		{
			GUILayout.Width(300f)
		});
		GUILayout.Label("by wittylunar", this.subStyle, new GUILayoutOption[]
		{
			GUILayout.Width(200f)
		});
		GUILayout.FlexibleSpace();
		GUILayout.EndHorizontal();
		GUILayout.EndArea();
		GUILayout.BeginArea(new Rect((float)(Screen.width / 2 - 320), 70f, 640f, 520f), this.boxStyle);
		GUILayout.BeginHorizontal(Array.Empty<GUILayoutOption>());
		if (GUILayout.Button("MAIN", (this.currentTab == AdminMenu.Tab.MAIN) ? this.btnActive : this.btnStyle, Array.Empty<GUILayoutOption>()))
		{
			this.currentTab = AdminMenu.Tab.MAIN;
		}
		if (GUILayout.Button("PLAYER", (this.currentTab == AdminMenu.Tab.PLAYER) ? this.btnActive : this.btnStyle, Array.Empty<GUILayoutOption>()))
		{
			this.currentTab = AdminMenu.Tab.PLAYER;
		}
		if (GUILayout.Button("AI", (this.currentTab == AdminMenu.Tab.AI) ? this.btnActive : this.btnStyle, Array.Empty<GUILayoutOption>()))
		{
			this.currentTab = AdminMenu.Tab.AI;
		}
		if (GUILayout.Button("SPAWN", (this.currentTab == AdminMenu.Tab.SPAWN) ? this.btnActive : this.btnStyle, Array.Empty<GUILayoutOption>()))
		{
			this.currentTab = AdminMenu.Tab.SPAWN;
		}
		GUILayout.EndHorizontal();
		GUILayout.BeginHorizontal(Array.Empty<GUILayoutOption>());
		if (GUILayout.Button("MAP", (this.currentTab == AdminMenu.Tab.MAP) ? this.btnActive : this.btnStyle, Array.Empty<GUILayoutOption>()))
		{
			this.currentTab = AdminMenu.Tab.MAP;
		}
		if (GUILayout.Button("WORLD", (this.currentTab == AdminMenu.Tab.WORLD) ? this.btnActive : this.btnStyle, Array.Empty<GUILayoutOption>()))
		{
			this.currentTab = AdminMenu.Tab.WORLD;
		}
		if (GUILayout.Button("CUT", (this.currentTab == AdminMenu.Tab.CUT) ? this.btnActive : this.btnStyle, Array.Empty<GUILayoutOption>()))
		{
			this.currentTab = AdminMenu.Tab.CUT;
		}
		if (GUILayout.Button("ESP", (this.currentTab == AdminMenu.Tab.ESP) ? this.btnActive : this.btnStyle, Array.Empty<GUILayoutOption>()))
		{
			this.currentTab = AdminMenu.Tab.ESP;
		}
		GUILayout.EndHorizontal();
		GUILayout.BeginHorizontal(Array.Empty<GUILayoutOption>());
		if (GUILayout.Button("PREFS", (this.currentTab == AdminMenu.Tab.PREFS) ? this.btnActive : this.btnStyle, Array.Empty<GUILayoutOption>()))
		{
			this.currentTab = AdminMenu.Tab.PREFS;
		}
		if (GUILayout.Button("RIGID", (this.currentTab == AdminMenu.Tab.RIGID) ? this.btnActive : this.btnStyle, Array.Empty<GUILayoutOption>()))
		{
			this.currentTab = AdminMenu.Tab.RIGID;
		}
		GUILayout.EndHorizontal();
		GUILayout.Space(5f);
		this.scrollPos = GUILayout.BeginScrollView(this.scrollPos, new GUILayoutOption[]
		{
			GUILayout.Height(420f)
		});
		try
		{
			if (this.currentTab == AdminMenu.Tab.MAIN)
			{
				GUILayout.Label("--- STATUS ---", this.headStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Scene: " + this.currentSceneName, this.labelStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Fly: " + (AdminMenu.flyEnabled ? "ON [Z]" : "OFF"), this.labelStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("God: " + (AdminMenu.godMode ? "ON [F2]" : "OFF"), this.labelStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Noclip: " + (AdminMenu.noclip ? "ON [X]" : "OFF"), this.labelStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("ESP: " + (AdminMenu.espEnabled ? "ON [E]" : "OFF"), this.labelStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Tracers: " + (AdminMenu.tracersEnabled ? "ON [T]" : "OFF"), this.labelStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("P = close menu", this.labelStyle, Array.Empty<GUILayoutOption>());
			}
			else if (this.currentTab == AdminMenu.Tab.PLAYER)
			{
				GUILayout.Label("--- MOVEMENT ---", this.headStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Fly: " + (AdminMenu.flyEnabled ? "ON [Z]" : "OFF"), this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					AdminMenu.flyEnabled = !AdminMenu.flyEnabled;
				}
				AdminMenu.flySpeed = GUILayout.HorizontalSlider(AdminMenu.flySpeed, 2f, 60f, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Fly speed: " + AdminMenu.flySpeed.ToString("F1"), this.labelStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Noclip: " + (AdminMenu.noclip ? "ON [X]" : "OFF"), this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					AdminMenu.noclip = !AdminMenu.noclip;
				}
				if (GUILayout.Button("God Mode: " + (AdminMenu.godMode ? "ON [F2]" : "OFF"), this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					AdminMenu.godMode = !AdminMenu.godMode;
				}
				GUILayout.Label("--- JUMP MULTIPLIER ---", this.headStyle, Array.Empty<GUILayoutOption>());
				AdminMenu.jumpMultiplier = GUILayout.HorizontalSlider(AdminMenu.jumpMultiplier, 1f, 10f, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Jump x" + AdminMenu.jumpMultiplier.ToString("F1"), this.labelStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Apply Jump Multiplier", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.ApplyJumpMultiplier();
				}
				GUILayout.Label("--- PLAYER SCALE ---", this.headStyle, Array.Empty<GUILayoutOption>());
				AdminMenu.playerScale = GUILayout.HorizontalSlider(AdminMenu.playerScale, 0.3f, 3f, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Scale: " + AdminMenu.playerScale.ToString("F2"), this.labelStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Apply Player Scale", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SetPlayerScale(AdminMenu.playerScale);
				}
				GUILayout.Label("--- CAMERA FOV ---", this.headStyle, Array.Empty<GUILayoutOption>());
				AdminMenu.fov = GUILayout.HorizontalSlider(AdminMenu.fov, 20f, 120f, Array.Empty<GUILayoutOption>());
				GUILayout.Label("FOV: " + AdminMenu.fov.ToString("F0"), this.labelStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Apply FOV", this.btnStyle, Array.Empty<GUILayoutOption>()) && Camera.main != null)
				{
					Camera.main.fieldOfView = AdminMenu.fov;
				}
				GUILayout.Label("--- TIME SCALE ---", this.headStyle, Array.Empty<GUILayoutOption>());
				AdminMenu.timeScale = GUILayout.HorizontalSlider(AdminMenu.timeScale, 0.1f, 2f, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Time: " + AdminMenu.timeScale.ToString("F2"), this.labelStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Apply Time Scale", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					Time.timeScale = AdminMenu.timeScale;
				}
				if (GUILayout.Button("Reset Time", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					Time.timeScale = 1f;
					AdminMenu.timeScale = 1f;
				}
			}
			else if (this.currentTab == AdminMenu.Tab.AI)
			{
				GUILayout.Label("--- AI INSPECTOR ---", this.headStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button(AdminMenu.disableAI ? "Enable ALL AI" : "Disable ALL AI", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.DisableAllAI();
				}
				if (GUILayout.Button("Destroy ALL Enemies", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.DestroyAllEnemies();
				}
				GUILayout.Label("--- ENEMY SCALE ---", this.headStyle, Array.Empty<GUILayoutOption>());
				float allEnemiesScale = GUILayout.HorizontalSlider(1f, 0.3f, 3f, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Enemy scale: " + allEnemiesScale.ToString("F2"), this.labelStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Apply Enemy Scale", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SetAllEnemiesScale(allEnemiesScale);
				}
				GUILayout.Space(5f);
				GUILayout.Label("Enemies (" + ((this.cachedEnemies != null) ? this.cachedEnemies.Length : 0) + "):", this.labelStyle, Array.Empty<GUILayoutOption>());
				if (this.cachedEnemies == null || this.cachedEnemies.Length == 0)
				{
					GUILayout.Label("(none)", this.labelStyle, Array.Empty<GUILayoutOption>());
				}
				foreach (MonoBehaviour monoBehaviour in this.cachedEnemies)
				{
					if (!(monoBehaviour == null) && !(monoBehaviour.gameObject == null))
					{
						GUILayout.BeginHorizontal(Array.Empty<GUILayoutOption>());
						GUILayout.Label((monoBehaviour.gameObject.name.Length > 12) ? (monoBehaviour.gameObject.name.Substring(0, 12) + "...") : monoBehaviour.gameObject.name, this.labelStyle, new GUILayoutOption[]
						{
							GUILayout.Width(130f)
						});
						if (GUILayout.Button("BLIND", this.btnStyle, Array.Empty<GUILayoutOption>()))
						{
							AdminMenu.SetFloat(monoBehaviour, "sightDistance", 0f);
							AdminMenu.SetFloat(monoBehaviour, "fieldOfView", 0f);
						}
						if (GUILayout.Button("FREEZE", this.btnStyle, Array.Empty<GUILayoutOption>()))
						{
							AdminMenu.SetBool(monoBehaviour, "isChasing", false);
							Component component = monoBehaviour.GetComponent("UnityEngine.AI.NavMeshAgent");
							if (component != null)
							{
								PropertyInfo property = component.GetType().GetProperty("speed");
								if (property != null)
								{
									property.SetValue(component, 0f, null);
								}
							}
						}
						if (GUILayout.Button("KILL", this.btnStyle, Array.Empty<GUILayoutOption>()))
						{
							UnityEngine.Object.Destroy(monoBehaviour.gameObject);
						}
						GUILayout.EndHorizontal();
					}
				}
			}
			else if (this.currentTab == AdminMenu.Tab.SPAWN)
			{
				GUILayout.Label("--- NPC SPAWN ---", this.headStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("N = spawn 1 | B (hold) = spawn many", this.labelStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("SPAWN 1 ENEMY", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SpawnOneEnemy();
				}
				this.spawnInterval = GUILayout.HorizontalSlider(this.spawnInterval, 0.05f, 1f, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Spawn interval: " + this.spawnInterval.ToString("F2") + "s", this.labelStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("--- UNITY PRIMITIVES ---", this.headStyle, Array.Empty<GUILayoutOption>());
				GUILayout.BeginHorizontal(Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("CUBE", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SpawnPrimitive(PrimitiveType.Cube);
				}
				if (GUILayout.Button("SPHERE", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SpawnPrimitive(PrimitiveType.Sphere);
				}
				if (GUILayout.Button("CYLINDER", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SpawnPrimitive(PrimitiveType.Cylinder);
				}
				GUILayout.EndHorizontal();
				GUILayout.BeginHorizontal(Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("PLANE", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SpawnPrimitive(PrimitiveType.Plane);
				}
				if (GUILayout.Button("CAPSULE", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SpawnPrimitive(PrimitiveType.Capsule);
				}
				if (GUILayout.Button("QUAD", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SpawnPrimitive(PrimitiveType.Quad);
				}
				GUILayout.EndHorizontal();
			}
			else if (this.currentTab == AdminMenu.Tab.MAP)
			{
				GUILayout.Label("--- TELEPORT ---", this.headStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("TP -> HOUSE", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.TP(143f, 2f, -18f, "House");
				}
				if (GUILayout.Button("TP -> HATCH", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.TP(441f, 11f, 250f, "Hatch");
				}
				if (GUILayout.Button("TP -> ARCHIVE", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.TP(545f, 18f, 237f, "Archive");
				}
				GUILayout.Space(5f);
				if (GUILayout.Button("Collect ALL items (GP)", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.CollectAllGP();
				}
				GUILayout.Space(5f);
				this.sceneInput = GUILayout.TextField(this.sceneInput, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("LOAD SCENE BY NAME", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					SceneManager.LoadScene(this.sceneInput);
				}
				if (GUILayout.Button("LOAD MP", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					SceneManager.LoadScene("MP");
				}
				if (GUILayout.Button("LOAD ESCAPE", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					SceneManager.LoadScene("Escape");
				}
			}
			else if (this.currentTab == AdminMenu.Tab.WORLD)
			{
				GUILayout.Label("--- WORLD ---", this.headStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Open ALL Doors", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.OpenAllDoors();
				}
				if (GUILayout.Button("Duplicate Items", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.DuplicateItems();
				}
			}
			else if (this.currentTab == AdminMenu.Tab.CUT)
			{
				GUILayout.Label("--- CUTSCENE ---", this.headStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Skip Current Animation", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SkipAnim();
				}
				if (GUILayout.Button("Force Animator State", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.ApplyAnimatorOverride();
				}
			}
			else if (this.currentTab == AdminMenu.Tab.ESP)
			{
				GUILayout.Label("--- VISUAL ---", this.headStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("ESP: " + (AdminMenu.espEnabled ? "ON [E]" : "OFF"), this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					AdminMenu.espEnabled = !AdminMenu.espEnabled;
				}
				if (GUILayout.Button("Tracers: " + (AdminMenu.tracersEnabled ? "ON [T]" : "OFF"), this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					AdminMenu.tracersEnabled = !AdminMenu.tracersEnabled;
				}
				GUILayout.Label("ESP draws red boxes around enemies", this.labelStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Tracers draws lines from screen center to enemies", this.labelStyle, Array.Empty<GUILayoutOption>());
			}
			else if (this.currentTab == AdminMenu.Tab.PREFS)
			{
				GUILayout.Label("--- PLAYERPREFS EDITOR ---", this.headStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Dump Known Keys to Log", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.DumpPlayerPrefs();
				}
				GUILayout.Label("--- UNLOCK ALL ENDINGS ---", this.headStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Unlock ALL Trophies/Endings", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					string[] array2 = new string[]
					{
						"Text2_Unlocked",
						"Text3_Unlocked",
						"Text4_Unlocked",
						"Text5_Unlocked",
						"Text6_Unlocked",
						"Text7_Unlocked",
						"Text8_Unlocked",
						"Text9_Unlocked",
						"Text10_Unlocked",
						"Text11_Unlocked"
					};
					for (int i = 0; i < array2.Length; i++)
					{
						PlayerPrefs.SetInt(array2[i], 1);
					}
					PlayerPrefs.Save();
					this.Log("All trophies unlocked");
				}
				GUILayout.Label("--- MANUAL EDIT ---", this.headStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Set Text5_Unlocked = 1", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SetPlayerPref("Text5_Unlocked", 1);
				}
				if (GUILayout.Button("Set Text8_Unlocked = 1", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.SetPlayerPref("Text8_Unlocked", 1);
				}
				if (GUILayout.Button("RESET ALL PROGRESS", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					PlayerPrefs.DeleteAll();
					PlayerPrefs.Save();
					this.Log("Progress reset");
				}
			}
			else if (this.currentTab == AdminMenu.Tab.RIGID)
			{
				GUILayout.Label("--- RIGIDBODY OVERRIDE ---", this.headStyle, Array.Empty<GUILayoutOption>());
				GUILayout.Label("Mass: 1 (normal) | 0.1 (light) | 100 (heavy)", this.labelStyle, Array.Empty<GUILayoutOption>());
				if (GUILayout.Button("Set Mass = 0.1 (light)", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.ApplyRigidbodyOverride(0.1f, true);
				}
				if (GUILayout.Button("Set Mass = 1 (normal)", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.ApplyRigidbodyOverride(1f, true);
				}
				if (GUILayout.Button("Set Mass = 100 (heavy)", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.ApplyRigidbodyOverride(100f, true);
				}
				if (GUILayout.Button("Disable Gravity", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.ApplyRigidbodyOverride(1f, false);
				}
				if (GUILayout.Button("Enable Gravity", this.btnStyle, Array.Empty<GUILayoutOption>()))
				{
					this.ApplyRigidbodyOverride(1f, true);
				}
			}
		}
		catch (Exception ex)
		{
			GUILayout.Label("ERROR: " + ex.Message, this.labelStyle, Array.Empty<GUILayoutOption>());
		}
		GUILayout.EndScrollView();
		GUILayout.EndArea();
	}

	// Token: 0x060000AC RID: 172 RVA: 0x00002E1B File Offset: 0x0000101B
	public AdminMenu()
	{
		this.sceneInput = "Blend";
		base..ctor();
	}

	// Token: 0x060000AD RID: 173 RVA: 0x00007140 File Offset: 0x00005340
	// Note: this type is marked as 'beforefieldinit'.
	static AdminMenu()
	{
		AdminMenu.flyEnabled = false;
		AdminMenu.godMode = false;
		AdminMenu.noclip = false;
		AdminMenu.disableAI = false;
		AdminMenu.espEnabled = false;
		AdminMenu.tracersEnabled = false;
		AdminMenu.flySpeed = 15f;
		AdminMenu.timeScale = 1f;
		AdminMenu.fov = 60f;
		AdminMenu.jumpMultiplier = 1f;
		AdminMenu.playerScale = 1f;
	}

	// Token: 0x060000AE RID: 174 RVA: 0x000071AC File Offset: 0x000053AC
	private static void SetInt(object o, string name, int v)
	{
		if (o == null)
		{
			return;
		}
		FieldInfo field = o.GetType().GetField(name, BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
		if (field != null && field.FieldType == typeof(int))
		{
			field.SetValue(o, v);
		}
	}

	// Token: 0x060000AF RID: 175 RVA: 0x00002E44 File Offset: 0x00001044
	private void Log(string msg)
	{
		Debug.Log("<color=red>Admin Console</color> | " + msg);
	}

	// Token: 0x060000B0 RID: 176 RVA: 0x000071FC File Offset: 0x000053FC
	private void DisableAllAI()
	{
		AdminMenu.disableAI = !AdminMenu.disableAI;
		if (this.cachedEnemies == null)
		{
			return;
		}
		foreach (MonoBehaviour monoBehaviour in this.cachedEnemies)
		{
			if (!(monoBehaviour == null))
			{
				AdminMenu.SetFloat(monoBehaviour, "sightDistance", AdminMenu.disableAI ? 0f : 15f);
				AdminMenu.SetFloat(monoBehaviour, "fieldOfView", AdminMenu.disableAI ? 0f : 60f);
				AdminMenu.SetBool(monoBehaviour, "isChasing", false);
				Component component = monoBehaviour.GetComponent("UnityEngine.AI.NavMeshAgent");
				if (component != null)
				{
					PropertyInfo property = component.GetType().GetProperty("speed");
					if (property != null)
					{
						property.SetValue(component, AdminMenu.disableAI ? 0f : 3f, null);
					}
				}
			}
		}
		this.Log("AI " + (AdminMenu.disableAI ? "OFF" : "ON"));
	}

	// Token: 0x060000B1 RID: 177 RVA: 0x00007308 File Offset: 0x00005508
	private void DestroyAllEnemies()
	{
		if (this.cachedEnemies == null)
		{
			return;
		}
		foreach (MonoBehaviour monoBehaviour in this.cachedEnemies)
		{
			if (monoBehaviour != null)
			{
				UnityEngine.Object.Destroy(monoBehaviour.gameObject);
			}
		}
		this.CacheEnemies();
		this.Log("All enemies destroyed");
	}

	// Token: 0x060000B2 RID: 178 RVA: 0x0000735C File Offset: 0x0000555C
	private void OpenAllDoors()
	{
		MonoBehaviour[] array = UnityEngine.Object.FindObjectsOfType<MonoBehaviour>();
		int num = 0;
		foreach (MonoBehaviour monoBehaviour in array)
		{
			if (monoBehaviour != null && monoBehaviour.GetType().Name == "Door")
			{
				FieldInfo field = monoBehaviour.GetType().GetField("open", BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
				if (field != null)
				{
					field.SetValue(monoBehaviour, true);
					num++;
				}
			}
		}
		this.Log("Doors opened: " + num);
	}

	// Token: 0x060000B3 RID: 179 RVA: 0x000073EC File Offset: 0x000055EC
	private void SkipAnim()
	{
		foreach (MonoBehaviour monoBehaviour in UnityEngine.Object.FindObjectsOfType<MonoBehaviour>())
		{
			if (monoBehaviour != null && monoBehaviour.GetType().Name == "AnimationSequenceController")
			{
				MethodInfo method = monoBehaviour.GetType().GetMethod("CompleteNow", BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
				if (method != null)
				{
					method.Invoke(monoBehaviour, null);
				}
			}
		}
		this.Log("Animation skipped");
	}

	// Token: 0x060000B4 RID: 180 RVA: 0x00007464 File Offset: 0x00005664
	private void CollectAllGP()
	{
		foreach (MonoBehaviour monoBehaviour in UnityEngine.Object.FindObjectsOfType<MonoBehaviour>())
		{
			if (monoBehaviour != null && monoBehaviour.GetType().Name == "ObjectCollectionManagerGP")
			{
				AdminMenu.SetInt(monoBehaviour, "collectedCount", 12);
				AdminMenu.SetBool(monoBehaviour, "allCollected", true);
				MethodInfo method = monoBehaviour.GetType().GetMethod("OnAllObjectsCollected", BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
				if (method != null)
				{
					method.Invoke(monoBehaviour, null);
				}
			}
		}
		this.Log("Items collected");
	}

	// Token: 0x060000B5 RID: 181 RVA: 0x000074F4 File Offset: 0x000056F4
	private void TP(float x, float y, float z, string name)
	{
		GameObject gameObject = GameObject.FindGameObjectWithTag("Player");
		if (gameObject != null)
		{
			gameObject.transform.position = new Vector3(x, y, z);
			this.Log("TP -> " + name);
		}
	}

	// Token: 0x060000B6 RID: 182 RVA: 0x0000753C File Offset: 0x0000573C
	private void EnsureStyles()
	{
		if (this.stylesReady)
		{
			return;
		}
		this.btnBg = this.MakeTex(2, 2, new Color(0.12f, 0.05f, 0.15f));
		this.btnActiveBg = this.MakeTex(2, 2, new Color(0.4f, 0.1f, 0.5f));
		this.bgTex = this.MakeTex(2, 2, new Color(0f, 0f, 0f, 0.5f));
		this.boxStyle = new GUIStyle(GUI.skin.box);
		this.boxStyle.normal.background = this.MakeTex(2, 2, new Color(0.05f, 0.05f, 0.08f, 0.95f));
		this.boxStyle.border = new RectOffset(4, 4, 4, 4);
		this.titleStyle = new GUIStyle(GUI.skin.label)
		{
			fontSize = 22,
			fontStyle = FontStyle.Bold,
			normal = 
			{
				textColor = new Color(1f, 0.2f, 0.5f)
			},
			alignment = TextAnchor.MiddleCenter
		};
		this.subStyle = new GUIStyle(GUI.skin.label)
		{
			fontSize = 12,
			fontStyle = FontStyle.Italic,
			normal = 
			{
				textColor = new Color(0.5f, 0.8f, 1f)
			},
			alignment = TextAnchor.MiddleCenter
		};
		this.headStyle = new GUIStyle(GUI.skin.label)
		{
			fontSize = 14,
			fontStyle = FontStyle.Bold,
			normal = 
			{
				textColor = new Color(0.3f, 1f, 0.7f)
			}
		};
		this.labelStyle = new GUIStyle(GUI.skin.label)
		{
			fontSize = 12,
			normal = 
			{
				textColor = new Color(0.9f, 0.9f, 0.9f)
			}
		};
		this.btnStyle = new GUIStyle(GUI.skin.button)
		{
			fontSize = 12,
			fontStyle = FontStyle.Bold,
			normal = 
			{
				textColor = Color.white,
				background = this.btnBg
			},
			hover = 
			{
				textColor = new Color(1f, 0.5f, 1f)
			}
		};
		this.btnActive = new GUIStyle(this.btnStyle)
		{
			normal = 
			{
				textColor = Color.white,
				background = this.btnActiveBg
			}
		};
		this.stylesReady = true;
	}

	// Token: 0x060000B7 RID: 183 RVA: 0x000077C8 File Offset: 0x000059C8
	private void CreateInGameCanvas()
	{
		if (this.adminCanvasObj != null)
		{
			return;
		}
		this.adminCanvasObj = new GameObject("AdminCanvas");
		UnityEngine.Object.DontDestroyOnLoad(this.adminCanvasObj);
		Canvas canvas = this.adminCanvasObj.AddComponent<Canvas>();
		canvas.renderMode = RenderMode.ScreenSpaceOverlay;
		canvas.sortingOrder = 9999;
		CanvasScaler canvasScaler = this.adminCanvasObj.AddComponent<CanvasScaler>();
		canvasScaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
		canvasScaler.referenceResolution = new Vector2(1920f, 1080f);
		this.adminCanvasObj.AddComponent<GraphicRaycaster>();
		GameObject gameObject = new GameObject("AdminText");
		gameObject.transform.SetParent(this.adminCanvasObj.transform, false);
		Text text = gameObject.AddComponent<Text>();
		text.text = "ADMIN CONSOLE";
		text.font = Resources.GetBuiltinResource<Font>("Arial.ttf");
		text.fontSize = 48;
		text.fontStyle = FontStyle.Bold;
		text.color = new Color(1f, 0.1f, 0.1f, 0.9f);
		text.alignment = TextAnchor.UpperCenter;
		text.horizontalOverflow = HorizontalWrapMode.Overflow;
		RectTransform component = gameObject.GetComponent<RectTransform>();
		component.anchorMin = new Vector2(0.5f, 1f);
		component.anchorMax = new Vector2(0.5f, 1f);
		component.pivot = new Vector2(0.5f, 1f);
		component.anchoredPosition = new Vector2(0f, -20f);
		component.sizeDelta = new Vector2(600f, 80f);
		GameObject gameObject2 = new GameObject("SubText");
		gameObject2.transform.SetParent(this.adminCanvasObj.transform, false);
		Text text2 = gameObject2.AddComponent<Text>();
		text2.text = "by wittylunar";
		text2.font = Resources.GetBuiltinResource<Font>("Arial.ttf");
		text2.fontSize = 20;
		text2.fontStyle = FontStyle.Italic;
		text2.color = new Color(0.7f, 0.2f, 1f, 0.9f);
		text2.alignment = TextAnchor.UpperCenter;
		RectTransform component2 = gameObject2.GetComponent<RectTransform>();
		component2.anchorMin = new Vector2(0.5f, 1f);
		component2.anchorMax = new Vector2(0.5f, 1f);
		component2.pivot = new Vector2(0.5f, 1f);
		component2.anchoredPosition = new Vector2(0f, -75f);
		component2.sizeDelta = new Vector2(400f, 40f);
	}

	// Token: 0x060000B8 RID: 184 RVA: 0x00007A1C File Offset: 0x00005C1C
	private void CreateHudCanvas()
	{
		if (this.hudCanvasObj != null)
		{
			return;
		}
		this.hudCanvasObj = new GameObject("HudCanvas");
		UnityEngine.Object.DontDestroyOnLoad(this.hudCanvasObj);
		Canvas canvas = this.hudCanvasObj.AddComponent<Canvas>();
		canvas.renderMode = RenderMode.ScreenSpaceOverlay;
		canvas.sortingOrder = 9998;
		CanvasScaler canvasScaler = this.hudCanvasObj.AddComponent<CanvasScaler>();
		canvasScaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
		canvasScaler.referenceResolution = new Vector2(1920f, 1080f);
		this.hudCanvasObj.AddComponent<GraphicRaycaster>();
		GameObject gameObject = new GameObject("HudText");
		gameObject.transform.SetParent(this.hudCanvasObj.transform, false);
		Text text = gameObject.AddComponent<Text>();
		text.name = "HudLabel";
		text.font = Resources.GetBuiltinResource<Font>("Arial.ttf");
		text.fontSize = 18;
		text.fontStyle = FontStyle.Bold;
		text.color = new Color(1f, 0.8f, 0.2f, 0.95f);
		text.alignment = TextAnchor.LowerLeft;
		RectTransform component = gameObject.GetComponent<RectTransform>();
		component.anchorMin = new Vector2(0f, 0f);
		component.anchorMax = new Vector2(0f, 0f);
		component.pivot = new Vector2(0f, 0f);
		component.anchoredPosition = new Vector2(20f, 20f);
		component.sizeDelta = new Vector2(500f, 60f);
	}

	// Token: 0x060000B9 RID: 185 RVA: 0x00007B84 File Offset: 0x00005D84
	private void UpdateHudText()
	{
		try
		{
			if (!(this.hudCanvasObj == null))
			{
				Text componentInChildren = this.hudCanvasObj.GetComponentInChildren<Text>();
				if (componentInChildren != null)
				{
					this.currentSceneName = SceneManager.GetActiveScene().name;
					componentInChildren.text = string.Concat(new string[]
					{
						"Scene: ",
						this.currentSceneName,
						" | Fly:",
						AdminMenu.flyEnabled ? "ON" : "OFF",
						" | Noclip:",
						AdminMenu.noclip ? "ON" : "OFF",
						" | God:",
						AdminMenu.godMode ? "ON" : "OFF"
					});
				}
			}
		}
		catch
		{
		}
	}

	// Token: 0x060000BA RID: 186 RVA: 0x00007C60 File Offset: 0x00005E60
	private void CacheEnemies()
	{
		try
		{
			MonoBehaviour[] array = UnityEngine.Object.FindObjectsOfType<MonoBehaviour>();
			List<MonoBehaviour> list = new List<MonoBehaviour>();
			foreach (MonoBehaviour monoBehaviour in array)
			{
				if (monoBehaviour != null && monoBehaviour.GetType().Name == "Enemy")
				{
					list.Add(monoBehaviour);
				}
			}
			this.cachedEnemies = list.ToArray();
		}
		catch
		{
			this.cachedEnemies = new MonoBehaviour[0];
		}
	}

	// Token: 0x060000BB RID: 187 RVA: 0x00007CE0 File Offset: 0x00005EE0
	private void ApplyNoclip()
	{
		try
		{
			GameObject gameObject = GameObject.FindGameObjectWithTag("Player");
			if (gameObject != null)
			{
				CharacterController component = gameObject.GetComponent<CharacterController>();
				if (component != null)
				{
					component.enabled = false;
				}
				Camera main = Camera.main;
				if (main != null)
				{
					Vector3 a = main.transform.right * Input.GetAxis("Horizontal") + main.transform.forward * Input.GetAxis("Vertical");
					if (Input.GetKey(KeyCode.Space))
					{
						a += Vector3.up;
					}
					if (Input.GetKey(KeyCode.LeftControl))
					{
						a += Vector3.down;
					}
					gameObject.transform.position += a * AdminMenu.flySpeed * Time.unscaledDeltaTime;
				}
			}
		}
		catch
		{
		}
	}

	// Token: 0x060000BC RID: 188 RVA: 0x00007DD4 File Offset: 0x00005FD4
	private void DrawESP()
	{
		try
		{
			if (AdminMenu.espEnabled || AdminMenu.tracersEnabled)
			{
				Camera main = Camera.main;
				if (!(main == null))
				{
					if (this.cachedEnemies != null)
					{
						foreach (MonoBehaviour monoBehaviour in this.cachedEnemies)
						{
							if (!(monoBehaviour == null) && !(monoBehaviour.gameObject == null))
							{
								Vector3 vector = main.WorldToScreenPoint(monoBehaviour.transform.position);
								if (vector.z >= 0f)
								{
									if (AdminMenu.espEnabled)
									{
										float num = 40f;
										GUI.Box(new Rect(vector.x - num / 2f, (float)Screen.height - vector.y - num / 2f, num, num * 1.5f), "", new GUIStyle
										{
											normal = 
											{
												background = this.MakeTex(2, 2, new Color(1f, 0f, 0f, 0.7f))
											}
										});
									}
									if (AdminMenu.tracersEnabled)
									{
										GL.Begin(1);
										GL.Color(new Color(1f, 0f, 0f, 0.8f));
										GL.Vertex3((float)(Screen.width / 2), (float)Screen.height, 0f);
										GL.Vertex3(vector.x, (float)Screen.height - vector.y, 0f);
										GL.End();
									}
								}
							}
						}
					}
				}
			}
		}
		catch
		{
		}
	}

	// Token: 0x060000BD RID: 189 RVA: 0x00007F7C File Offset: 0x0000617C
	private void SpawnOneEnemy()
	{
		GameObject gameObject = null;
		try
		{
			foreach (MonoBehaviour monoBehaviour in UnityEngine.Object.FindObjectsOfType<MonoBehaviour>())
			{
				if (monoBehaviour != null && monoBehaviour.GetType().Name == "Enemy")
				{
					gameObject = monoBehaviour.gameObject;
					break;
				}
			}
		}
		catch
		{
		}
		if (gameObject == null)
		{
			if (!this.noEnemyTemplateShown)
			{
				this.Log("Нет врага-шаблона в этой сцене");
				this.noEnemyTemplateShown = true;
			}
			return;
		}
		this.noEnemyTemplateShown = false;
		Vector3 position = gameObject.transform.position;
		GameObject gameObject2 = GameObject.FindGameObjectWithTag("Player");
		if (gameObject2 != null)
		{
			position = gameObject2.transform.position;
		}
		Vector3 position2 = position + new Vector3(UnityEngine.Random.Range(-3f, 3f), 1f, UnityEngine.Random.Range(-3f, 3f));
		UnityEngine.Object.Instantiate<GameObject>(gameObject, position2, gameObject.transform.rotation).name = "Enemy_spawn";
		this.Log("NPC spawned");
	}

	// Token: 0x060000BE RID: 190 RVA: 0x0000809C File Offset: 0x0000629C
	private void SpawnPrimitive(PrimitiveType type)
	{
		GameObject gameObject = GameObject.FindGameObjectWithTag("Player");
		Vector3 position = (gameObject != null) ? (gameObject.transform.position + gameObject.transform.forward * 3f) : Vector3.zero;
		GameObject gameObject2 = GameObject.CreatePrimitive(type);
		gameObject2.transform.position = position;
		gameObject2.name = "Prim_" + type;
		this.Log("Spawned " + type);
	}

	// Token: 0x060000BF RID: 191 RVA: 0x00008128 File Offset: 0x00006328
	private void SetPlayerScale(float s)
	{
		GameObject gameObject = GameObject.FindGameObjectWithTag("Player");
		if (gameObject != null)
		{
			gameObject.transform.localScale = new Vector3(s, s, s);
		}
	}

	// Token: 0x060000C0 RID: 192 RVA: 0x0000815C File Offset: 0x0000635C
	private void SetAllEnemiesScale(float s)
	{
		if (this.cachedEnemies == null)
		{
			return;
		}
		foreach (MonoBehaviour monoBehaviour in this.cachedEnemies)
		{
			if (monoBehaviour != null && monoBehaviour.gameObject != null)
			{
				monoBehaviour.transform.localScale = new Vector3(s, s, s);
			}
		}
	}

	// Token: 0x060000C1 RID: 193 RVA: 0x000081B8 File Offset: 0x000063B8
	private void ApplyJumpMultiplier()
	{
		foreach (MonoBehaviour monoBehaviour in UnityEngine.Object.FindObjectsOfType<MonoBehaviour>())
		{
			if (!(monoBehaviour == null))
			{
				string name = monoBehaviour.GetType().Name;
				if (name == "PlayerController" || name == "FPSController")
				{
					FieldInfo field = monoBehaviour.GetType().GetField("jumpForce", BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
					if (field != null && field.FieldType == typeof(float))
					{
						field.SetValue(monoBehaviour, 8f * AdminMenu.jumpMultiplier);
					}
					FieldInfo field2 = monoBehaviour.GetType().GetField("jumpSpeed", BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
					if (field2 != null && field2.FieldType == typeof(float))
					{
						field2.SetValue(monoBehaviour, 8f * AdminMenu.jumpMultiplier);
					}
				}
			}
		}
	}

	// Token: 0x060000C2 RID: 194 RVA: 0x000082B4 File Offset: 0x000064B4
	private void ApplyRigidbodyOverride(float mass, bool useGravity)
	{
		foreach (Rigidbody rigidbody in UnityEngine.Object.FindObjectsOfType<Rigidbody>())
		{
			if (rigidbody != null)
			{
				rigidbody.mass = mass;
				rigidbody.useGravity = useGravity;
			}
		}
		this.Log(string.Concat(new object[]
		{
			"Rigidbody mass=",
			mass,
			" gravity=",
			useGravity.ToString()
		}));
	}

	// Token: 0x060000C3 RID: 195 RVA: 0x00008324 File Offset: 0x00006524
	private void ApplyAnimatorOverride()
	{
		foreach (Animator animator in UnityEngine.Object.FindObjectsOfType<Animator>())
		{
			if (animator != null && animator.runtimeAnimatorController != null)
			{
				foreach (AnimationClip animationClip in animator.runtimeAnimatorController.animationClips)
				{
					if (animationClip != null && animationClip.name.ToLower().Contains("idle"))
					{
						animator.Play(animationClip.name);
						this.Log("Animator forced: " + animationClip.name);
						return;
					}
				}
			}
		}
	}

	// Token: 0x060000C4 RID: 196 RVA: 0x000083D4 File Offset: 0x000065D4
	private void DuplicateItems()
	{
		foreach (MonoBehaviour monoBehaviour in UnityEngine.Object.FindObjectsOfType<MonoBehaviour>())
		{
			if (!(monoBehaviour == null) && (monoBehaviour.GetType().Name == "ObjectCollectionManager" || monoBehaviour.GetType().Name == "ObjectCollectionManagerGP"))
			{
				FieldInfo field = monoBehaviour.GetType().GetField("objectsToCollect", BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
				if (field != null && field.FieldType == typeof(GameObject[]))
				{
					GameObject[] array2 = (GameObject[])field.GetValue(monoBehaviour);
					if (array2 != null && array2.Length != 0)
					{
						List<GameObject> list = new List<GameObject>(array2);
						foreach (GameObject gameObject in array2)
						{
							if (gameObject != null)
							{
								list.Add(UnityEngine.Object.Instantiate<GameObject>(gameObject));
							}
						}
						field.SetValue(monoBehaviour, list.ToArray());
						this.Log("Items duplicated: " + list.Count);
						return;
					}
				}
			}
		}
	}

	// Token: 0x060000C5 RID: 197 RVA: 0x000084F8 File Offset: 0x000066F8
	private void DumpPlayerPrefs()
	{
		foreach (string text in new string[]
		{
			"Text2_Unlocked",
			"Text3_Unlocked",
			"Text4_Unlocked",
			"Text5_Unlocked",
			"Text6_Unlocked",
			"Text7_Unlocked",
			"Text8_Unlocked",
			"Text9_Unlocked",
			"Text10_Unlocked",
			"Text11_Unlocked",
			"QualitySetting",
			"ResolutionIndex",
			"Fullscreen",
			"VSync",
			"AntiAliasing",
			"ShadowQuality"
		})
		{
			if (PlayerPrefs.HasKey(text))
			{
				int @int = PlayerPrefs.GetInt(text, 0);
				Debug.Log(string.Concat(new object[]
				{
					"<color=red>Admin Console</color> | PlayerPrefs[",
					text,
					"] = ",
					@int
				}));
			}
		}
	}

	// Token: 0x060000C6 RID: 198 RVA: 0x00002E56 File Offset: 0x00001056
	private void SetPlayerPref(string key, int value)
	{
		PlayerPrefs.SetInt(key, value);
		PlayerPrefs.Save();
		this.Log(string.Concat(new object[]
		{
			"PlayerPrefs[",
			key,
			"] = ",
			value
		}));
	}

	// Token: 0x060000C7 RID: 199 RVA: 0x00002E92 File Offset: 0x00001092
	private void OnDestroy()
	{
		if (AdminMenu.instance == this)
		{
			AdminMenu.instance = null;
		}
	}

	// Token: 0x040000C1 RID: 193
	public static bool flyEnabled;

	// Token: 0x040000C2 RID: 194
	public static bool godMode;

	// Token: 0x040000C3 RID: 195
	public static bool disableAI;

	// Token: 0x040000C4 RID: 196
	public static float flySpeed;

	// Token: 0x040000C5 RID: 197
	public static bool menuOpen = false;

	// Token: 0x040000C6 RID: 198
	private string sceneInput;

	// Token: 0x040000C7 RID: 199
	private GUIStyle titleStyle;

	// Token: 0x040000C8 RID: 200
	private GUIStyle labelStyle;

	// Token: 0x040000C9 RID: 201
	private GUIStyle boxStyle;

	// Token: 0x040000CA RID: 202
	private AdminMenu.Tab currentTab;

	// Token: 0x040000CB RID: 203
	private GUIStyle subStyle;

	// Token: 0x040000CC RID: 204
	private GUIStyle btnStyle;

	// Token: 0x040000CD RID: 205
	private GUIStyle btnActive;

	// Token: 0x040000CE RID: 206
	private GUIStyle headStyle;

	// Token: 0x040000CF RID: 207
	private Vector2 scrollPos;

	// Token: 0x040000D0 RID: 208
	private bool stylesReady;

	// Token: 0x040000D1 RID: 209
	public static bool noclip;

	// Token: 0x040000D2 RID: 210
	public static bool espEnabled;

	// Token: 0x040000D3 RID: 211
	public static bool tracersEnabled;

	// Token: 0x040000D4 RID: 212
	public static float timeScale;

	// Token: 0x040000D5 RID: 213
	public static float fov;

	// Token: 0x040000D6 RID: 214
	public static float jumpMultiplier;

	// Token: 0x040000D7 RID: 215
	public static float playerScale;

	// Token: 0x040000D8 RID: 216
	private MonoBehaviour[] cachedEnemies;

	// Token: 0x040000D9 RID: 217
	private float lastEnemyCacheTime;

	// Token: 0x040000DA RID: 218
	private string currentSceneName = "";

	// Token: 0x040000DB RID: 219
	private float lastSceneCheckTime;

	// Token: 0x040000DC RID: 220
	private GameObject adminCanvasObj;

	// Token: 0x040000DD RID: 221
	private GameObject hudCanvasObj;

	// Token: 0x040000DE RID: 222
	private Texture2D bgTex;

	// Token: 0x040000DF RID: 223
	private Texture2D btnBg;

	// Token: 0x040000E0 RID: 224
	private Texture2D btnActiveBg;

	// Token: 0x040000E1 RID: 225
	private float spawnTimer;

	// Token: 0x040000E2 RID: 226
	private float spawnInterval = 0.3f;

	// Token: 0x040000E3 RID: 227
	private static AdminMenu instance;

	// Token: 0x040000E4 RID: 228
	private bool noEnemyTemplateShown;

	// Token: 0x02000020 RID: 32
	private enum Tab
	{
		// Token: 0x040000E6 RID: 230
		MAIN,
		// Token: 0x040000E7 RID: 231
		PLAYER,
		// Token: 0x040000E8 RID: 232
		AI,
		// Token: 0x040000E9 RID: 233
		MAP = 4,
		// Token: 0x040000EA RID: 234
		WORLD,
		// Token: 0x040000EB RID: 235
		CUT,
		// Token: 0x040000EC RID: 236
		SPAWN = 3,
		// Token: 0x040000ED RID: 237
		ESP = 7,
		// Token: 0x040000EE RID: 238
		PREFS,
		// Token: 0x040000EF RID: 239
		RIGID
	}
}
