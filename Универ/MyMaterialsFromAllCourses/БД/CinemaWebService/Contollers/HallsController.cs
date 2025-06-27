// using Microsoft.AspNetCore.Mvc;
// using Microsoft.EntityFrameworkCore;
// using CinemaWebService.Data;
// using CinemaWebService.Models;
//
// namespace CinemaWebService.Controllers
// {
//     [ApiController]
//     [Route("api/[controller]")]
//     public class HallsController : ControllerBase
//     {
//         private readonly ApplicationDbContext _context;
//
//         public HallsController(ApplicationDbContext context)
//         {
//             _context = context;
//         }
//         
//         [HttpGet]
//         public async Task<ActionResult<IEnumerable<Hall>>> GetHalls()
//         {
//             return await _context.Halls.ToListAsync();
//         }
//         
//         [HttpGet("{id}")]
//         public async Task<ActionResult<Hall>> GetHall(int id)
//         {
//             var hall = await _context.Halls
//                 // .Include(h => h.Seats)
//                 // .Include(h => h.Sessions)
//                 .FirstOrDefaultAsync(h => h.HallId == id);
//
//             if (hall == null)
//             {
//                 return NotFound();
//             }
//
//             return hall;
//         }
//         
//         [HttpPost]
//         public async Task<ActionResult<Hall>> PostHall(Hall hall)
//         {
//             _context.Halls.Add(hall);
//             await _context.SaveChangesAsync();
//
//             return CreatedAtAction(nameof(GetHall), new { id = hall.HallId }, hall);
//         }
//         
//         [HttpPut("{id}")]
//         public async Task<IActionResult> PutHall(int id, Hall hall)
//         {
//             if (id != hall.HallId)
//             {
//                 return BadRequest("Идентификаторы не совпадают");
//             }
//
//             _context.Entry(hall).State = EntityState.Modified;
//
//             try
//             {
//                 await _context.SaveChangesAsync();
//             }
//             catch (DbUpdateConcurrencyException)
//             {
//                 if (!HallExists(id))
//                 {
//                     return NotFound();
//                 }
//                 throw;
//             }
//
//             return NoContent();
//         }
//         
//         [HttpDelete("{id}")]
//         public async Task<IActionResult> DeleteHall(int id)
//         {
//             var hall = await _context.Halls.FindAsync(id);
//             if (hall == null)
//             {
//                 return NotFound();
//             }
//
//             _context.Halls.Remove(hall);
//             await _context.SaveChangesAsync();
//
//             return NoContent();
//         }
//
//         private bool HallExists(int id)
//         {
//             return _context.Halls.Any(e => e.HallId == id);
//         }
//     }
// }

using Microsoft.AspNetCore.Mvc;
using MongoDB.Driver;
using CinemaWebService.Models;
using MongoDB.Driver.Linq;

namespace CinemaWebService.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class HallsController : ControllerBase
    {
        private readonly IMongoCollection<Hall> _hallsCollection;

        public HallsController(IMongoDatabase database)
        {
            _hallsCollection = database.GetCollection<Hall>("halls");
        }
        
        [HttpGet]
        public async Task<ActionResult<IEnumerable<Hall>>> GetHalls()
        {
            var halls = await _hallsCollection.Find(_ => true).ToListAsync();
            return Ok(halls);
        }
        
        [HttpGet("{id}")]
        public async Task<ActionResult<Hall>> GetHall(int id)
        {
            var hall = await _hallsCollection.Find(h => h.HallId == id).FirstOrDefaultAsync();
            if (hall == null)
            {
                return NotFound();
            }

            return Ok(hall);
        }
        
        [HttpPost]
        public async Task<ActionResult<Hall>> PostHall(Hall hall)
        {
            var maxId = await _hallsCollection.AsQueryable().MaxAsync(h => (int?)h.HallId) ?? 0;
            hall.HallId = maxId + 1;

            await _hallsCollection.InsertOneAsync(hall);
            return CreatedAtAction(nameof(GetHall), new { id = hall.HallId }, hall);
        }
        
        [HttpPut("{id}")]
        public async Task<IActionResult> PutHall(int id, Hall hall)
        {
            if (id != hall.HallId)
            {
                return BadRequest("Идентификаторы не совпадают");
            }

            var replaceResult = await _hallsCollection.ReplaceOneAsync(h => h.HallId == id, hall);
            if (replaceResult.MatchedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }
        
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteHall(int id)
        {
            var deleteResult = await _hallsCollection.DeleteOneAsync(h => h.HallId == id);
            if (deleteResult.DeletedCount == 0)
            {
                return NotFound();
            }
            return NoContent();
        }
    }
}