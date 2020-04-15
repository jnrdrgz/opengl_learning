#include "particle.h"

Particle::Particle(int id) : id{id} {
	
	totalLife = 1.0f;
	life = 1.0f;

	alpha = 1.0f;
	size = 1.0f;

	bounciness = 0.9f;

	active = true;

	lastTime = -1;
}

void Particle::Update(long time) {

	if (active == false) {
		return;
	}


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	if (lastTime == -1) {
		lastTime = time;
	}

	float change = (float)(time - lastTime) / 1000.0f;

	velocity += acceleration*change;
	position += velocity*change;


	if (position.y < 0.0f) {
		velocity.y = velocity.y + -bounciness;
		position.y = 0.0f;
	}

	const float fadeTime = 0.2f;

	if (totalLife - life < fadeTime) {
		
		glColor4f(color.x, color.y, color.z, (totalLife-life)/fadeTime*alpha);

		

	} else if(life<1.0f) {
		glColor4f(color.x, color.y, color.z, life*alpha);
	} else {
		glColor4f(color.x, color.y, color.z, alpha);
	}

	glTranslatef(position.x, position.y, position.z);
	
	float modelView[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j) {
				modelView[i * 4 + j] = 1.0f;
			}
			else {
				modelView[i * 4 + j] = 0.0f;
			}
		}
	}

	glLoadMatrixf(modelView);


	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(1, 1); glVertex3f(size, size, 0);
		glTexCoord2f(0, 1); glVertex3f(-size, size, 0);
		glTexCoord2f(1, 0); glVertex3f(size, -size, 0);
		glTexCoord2f(0, 0); glVertex3f(-size, -size, 0);

	glEnd();

	life -= change;

	if (life <= 0.0f) {
		active = false;
	}

	lastTime = time;

	glPopMatrix();
}