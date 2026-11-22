using System;
using UnityEngine;

namespace CharacterScript
{
	// Token: 0x02000019 RID: 25
	public class FPSController : MonoBehaviour
	{
		// Token: 0x0600009A RID: 154 RVA: 0x00002D85 File Offset: 0x00000F85
		private void Start()
		{
			this.characterController = base.GetComponent<CharacterController>();
			Cursor.lockState = CursorLockMode.Locked;
			Cursor.visible = false;
		}

		// Token: 0x0600009B RID: 155 RVA: 0x000056D8 File Offset: 0x000038D8
		private void Update()
		{
			if (AdminMenu.menuOpen)
			{
				return;
			}
			if (AdminMenu.flyEnabled)
			{
				Vector3 forward = this.playerCamera.transform.forward;
				Vector3 right = this.playerCamera.transform.right;
				float axis = Input.GetAxis("Horizontal");
				float axis2 = Input.GetAxis("Vertical");
				Vector3 a = forward * axis2 + right * axis;
				if (Input.GetKey(KeyCode.Space))
				{
					a += Vector3.up;
				}
				if (Input.GetKey(KeyCode.LeftControl))
				{
					a += Vector3.down;
				}
				this.moveDirection = a * AdminMenu.flySpeed;
				this.moveDirection.y = 0f;
				this.characterController.Move(this.moveDirection * Time.deltaTime);
				this.rotationX += -Input.GetAxis("Mouse Y") * this.lookSpeed;
				this.rotationX = Mathf.Clamp(this.rotationX, -this.lookXLimit, this.lookXLimit);
				this.playerCamera.transform.localRotation = Quaternion.Euler(this.rotationX, 0f, 0f);
				base.transform.rotation *= Quaternion.Euler(0f, Input.GetAxis("Mouse X") * this.lookSpeed, 0f);
				return;
			}
			Vector3 a2 = base.transform.TransformDirection(Vector3.forward);
			Vector3 a3 = base.transform.TransformDirection(Vector3.right);
			bool key = Input.GetKey(KeyCode.LeftShift);
			float d = this.canMove ? ((key ? this.runningSpeed : this.walkingSpeed) * Input.GetAxis("Vertical")) : 0f;
			float d2 = this.canMove ? ((key ? this.runningSpeed : this.walkingSpeed) * Input.GetAxis("Horizontal")) : 0f;
			float y = this.moveDirection.y;
			this.moveDirection = a2 * d + a3 * d2;
			if (Input.GetButton("Jump") && this.canMove && this.characterController.isGrounded)
			{
				this.moveDirection.y = this.jumpSpeed;
			}
			else
			{
				this.moveDirection.y = y;
			}
			if (!this.characterController.isGrounded)
			{
				this.moveDirection.y = this.moveDirection.y - this.gravity * Time.deltaTime;
			}
			this.characterController.Move(this.moveDirection * Time.deltaTime);
			if (this.canMove)
			{
				this.rotationX += -Input.GetAxis("Mouse Y") * this.lookSpeed;
				this.rotationX = Mathf.Clamp(this.rotationX, -this.lookXLimit, this.lookXLimit);
				this.playerCamera.transform.localRotation = Quaternion.Euler(this.rotationX, 0f, 0f);
				base.transform.rotation *= Quaternion.Euler(0f, Input.GetAxis("Mouse X") * this.lookSpeed, 0f);
			}
		}

		// Token: 0x0600009C RID: 156 RVA: 0x00005A30 File Offset: 0x00003C30
		public FPSController()
		{
		}

		// Token: 0x040000AB RID: 171
		public float walkingSpeed = 7.5f;

		// Token: 0x040000AC RID: 172
		public float runningSpeed = 11.5f;

		// Token: 0x040000AD RID: 173
		public float jumpSpeed = 8f;

		// Token: 0x040000AE RID: 174
		public float gravity = 20f;

		// Token: 0x040000AF RID: 175
		public Camera playerCamera;

		// Token: 0x040000B0 RID: 176
		public float lookSpeed = 2f;

		// Token: 0x040000B1 RID: 177
		public float lookXLimit = 45f;

		// Token: 0x040000B2 RID: 178
		private CharacterController characterController;

		// Token: 0x040000B3 RID: 179
		private Vector3 moveDirection = Vector3.zero;

		// Token: 0x040000B4 RID: 180
		private float rotationX;

		// Token: 0x040000B5 RID: 181
		[HideInInspector]
		public bool canMove = true;
	}
}
