# Vanagandar

This project was started as a graduation project for designers, meaning they could work fulltime on the project. I joined in as one of 3 programmers helping out with the project while still having courses on the side. 

## Setting up the team for success  
_A slightly clickbait-y title with just a **hint** of hubris, but it describes how I worked within the project._ 

With a team full of designers that could work full time and programmers that could only work part time, we knew that a lot of work would fall to our technical designers. The problem with this is that larger systems sometimes tend to look like something straight from the kitchen in an Italian restaurant. Especially when fully built in blueprints. To prevent this I built the base for weapons in our game so that designers could easily create and add behavior-blueprints (aka Components) to them without having to poke around in the weapons themselves. Recoil can be created for our weapons by simply creating a blueprint and defining what should happen in an inherited function. Then we can simply select to add this UObject class in a dropdown menu in our weapon and recoil just works (assuming the recoil itself is done right

![DropDownSelection](https://github.com/LostmyCigar/Vanagandar/assets/60781151/16117c2d-4971-4b09-adc3-c5f7e8cdff0e)  
_The dropdown menu we use when adding behvaiours to a weapon_  


The system is built around the fact that weapons are (as with a surprising amount of other common game mechanics) essentially split into three parts:

 - When should I do thing?
 - How do I do thing?
 - What happens now that I have done thing?

(Thing, in this case, being shooting)

 - The **WHEN**  
Constraints we call them, the class that handles if a weapon is allowed to shoot or not when the player sends us shoot input. Here we place things such as Fire Rate and Ammunition. When the weapon wants to know if it is allowed to shoot, it simply CheckConstraints() if they all return true (meaning we are allowed ofcourse).

![Vgdnr_TryFire_Func](https://github.com/LostmyCigar/Vanagandar/assets/60781151/11a2dbd8-e912-41b3-b315-a3228aba46f4)


 - The **HOW**  
This part is broken down into two classes: Aim modifiers and Bullet Spawners.  
    - Aim Modifiers affect where we shoot and include things such as movement error and spread. These modifiers gets passed a Vector AimDirection by reference that they modify in the way they want before sending it forth to BulletSpawners.  
    - The actual shooting. The single trace, multi trace or multiple multi traces that we wanna do when shooting. All put together under the collective name “Bullet Spawner”. Unlike our constraints or Aim modifiers, a weapon can only have one bullet spawner, if we want to shoot multiple bullets we do that inside our bullet spawner. This is meant for
  
![Vgdnr_Fire_Func](https://github.com/LostmyCigar/Vanagandar/assets/60781151/ddd2a3ed-c5af-4871-9238-c0a4e18d9014)


## The Code 



![Vgdnr_BulletModifier_Instanced](https://github.com/LostmyCigar/Vanagandar/assets/60781151/1ef279f4-4087-4c5c-9160-05b21425b7de)

![WeaponBehaviour_Header](https://github.com/LostmyCigar/Vanagandar/assets/60781151/6dfd6510-19f2-422f-98b7-eafd2a368c5c)

![AimModifier_header](https://github.com/LostmyCigar/Vanagandar/assets/60781151/bafad3c8-0b49-4acd-ab6f-ed8cb7a1964f)



When developing tools for designers to work with I focus on three points:

 - Flexibility 
Designers should be able to use the tool to create whatever they set their mind too
(Few limitations)

 - Scalability
We should be able to add behaviors and functionality without having to modify or accidently break existing ones. More or less, follow the Open-Closed principle.

 - Understandability
This one is tricky compared to the other two points, as it is not as much related to code. Designers must be able to easily understand the tool and be able to quickly pick it up and use it to iterate on their designs.

             

While scalability and flexibility (to some degree) can be achieved by just making sure your following SOLID, understandability is not. 



