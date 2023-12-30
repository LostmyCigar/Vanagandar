# Vanagandar

This project was started as a graduation project for designers, meaning they could work fulltime on the project. I joined in as one of 3 programmers helping out with the project while still having courses on the side. 

## Setting up the team for success

_A slightly clickbait-y title with just a **hint** of hubris, but it describes how I worked within the project._ 

With a team full of designers that could work full time and programmers that could only work part time, we knew that a lot of work would fall to our (very skilled) technical designers. The problem with this is that larger systems sometimes tend to look like something straight from the kitchen in an Italian restaurant. Especially when fully built in blueprints. To prevent this I built the base for weapons in our game so that designers could easily create and add behavior-blueprints to them without having to poke around in the weapons themselves (aka Components). Recoil can be created for our weapons by simply creating a blueprint and defining what should happen in an inherited function. Then we can simply select to add this UObject class in a dropdown menu in our weapon and recoil just works (assuming the recoil itself is done right). 

The system is built around the fact that weapons are (as with a surprising amount of other common game mechanics) essentially split into three parts:

 - When should I do thing?
 - How do I do thing?
 - What happens now that I have done thing?

(Thing, in this case, being shooting)

 - The **WHEN**  
Constraints we call them, the class that handles if a weapon is allowed to shoot or not when the player sends us shoot input. Here we place things such as Fire Rate and Ammunition. When the weapon wants to know if it is allowed to shoot, it simply CheckConstraints() if they all return true (meaning we are allowed ofcourse).  

 - The **HOW**  
This part is broken down into two classes: Aim modifiers and Bullet Spawners.  
    - Aim Modifiers affect where we shoot and include things such as movement error and spread. These modifiers gets passed a Vector AimDirection by reference that they modify in the way they want before sending it forth to BulletSpawners.  
    - The actual shooting. The single trace, multi trace or multiple multi traces that we wanna do when shooting. All put together under the collective name “Bullet Spawner”. Unlike our constraints or Aim modifiers, a weapon can only have one bullet spawner, if we want to shoot multiple bullets we do that inside our bullet spawner. This is meant for 











When developing tools for designers to work with I focus on three points:

 - Flexibility 
Designers should be able to use the tool to create whatever they set their mind too
(Few limitations)

 - Scalability
We should be able to add behaviors and functionality without having to modify or accidently break existing ones. More or less, follow the Open-Closed principle.

 - Understandability
This one is tricky compared to the other two points, as it is not as much related to code. Designers must be able to easily understand the tool and be able to quickly pick it up and use it to iterate on their designs.

             

While scalability and flexibility (to some degree) can be achieved by just making sure your following SOLID, understandability is not. 



