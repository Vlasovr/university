// using Microsoft.AspNetCore.Mvc;
// using Microsoft.EntityFrameworkCore;
// using CinemaWebService.Data;
// using CinemaWebService.Models;
//
// namespace CinemaWebService.Controllers
// {
//     [ApiController]
//     [Route("api/[controller]")]
//     public class SeatsController : ControllerBase
//     {
//         private readonly ApplicationDbContext _context;
//
//         public SeatsController(ApplicationDbContext context)
//         {
//             _context = context;
//         }
//         
//         [HttpGet]
//         public async Task<ActionResult<IEnumerable<Seat>>> GetSeats()
//         {
//             return await _context.Seats
//                 // .Include(s => s.Hall)
//                 .ToListAsync();
//         }
//         
//         [HttpGet("{id}")]
//         public async Task<ActionResult<Seat>> GetSeat(int id)
//         {
//             var seat = await _context.Seats
//                 // .Include(s => s.Hall)
//                 .FirstOrDefaultAsync(s => s.SeatId == id);
//
//             if (seat == null)
//             {
//                 return NotFound();
//             }
//
//             return seat;
//         }
//         
//         [HttpPost]
//         public async Task<ActionResult<Seat>> PostSeat(Seat seat)
//         {
//             _context.Seats.Add(seat);
//             await _context.SaveChangesAsync();
//
//             return CreatedAtAction(nameof(GetSeat), new { id = seat.SeatId }, seat);
//         }
//         
//         [HttpPut("{id}")]
//         public async Task<IActionResult> PutSeat(int id, Seat seat)
//         {
//             if (id != seat.SeatId)
//             {
//                 return BadRequest("Идентификаторы не совпадают");
//             }
//
//             _context.Entry(seat).State = EntityState.Modified;
//
//             try
//             {
//                 await _context.SaveChangesAsync();
//             }
//             catch (DbUpdateConcurrencyException)
//             {
//                 if (!SeatExists(id))
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
//         public async Task<IActionResult> DeleteSeat(int id)
//         {
//             var seat = await _context.Seats.FindAsync(id);
//             if (seat == null)
//             {
//                 return NotFound();
//             }
//
//             _context.Seats.Remove(seat);
//             await _context.SaveChangesAsync();
//
//             return NoContent();
//         }
//
//         private bool SeatExists(int id)
//         {
//             return _context.Seats.Any(s => s.SeatId == id);
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
    public class SeatsController : ControllerBase
    {
        private readonly IMongoCollection<Seat> _seatsCollection;

        public SeatsController(IMongoDatabase database)
        {
            _seatsCollection = database.GetCollection<Seat>("seats");
        }
        
        [HttpGet]
        public async Task<ActionResult<IEnumerable<Seat>>> GetSeats()
        {
            var seats = await _seatsCollection.Find(_ => true).ToListAsync();
            return Ok(seats);
        }
        
        [HttpGet("{id}")]
        public async Task<ActionResult<Seat>> GetSeat(int id)
        {
            var seat = await _seatsCollection.Find(s => s.SeatId == id).FirstOrDefaultAsync();
            if (seat == null)
            {
                return NotFound();
            }

            return Ok(seat);
        }
        
        [HttpPost]
        public async Task<ActionResult<Seat>> PostSeat(Seat seat)
        {
            // Простейший вариант автоинкремента
            var maxId = await _seatsCollection.AsQueryable().MaxAsync(s => (int?)s.SeatId) ?? 0;
            seat.SeatId = maxId + 1;

            await _seatsCollection.InsertOneAsync(seat);

            return CreatedAtAction(nameof(GetSeat), new { id = seat.SeatId }, seat);
        }
        
        [HttpPut("{id}")]
        public async Task<IActionResult> PutSeat(int id, Seat seat)
        {
            if (id != seat.SeatId)
            {
                return BadRequest("Идентификаторы не совпадают");
            }

            var replaceResult = await _seatsCollection.ReplaceOneAsync(s => s.SeatId == id, seat);
            if (replaceResult.MatchedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }

        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteSeat(int id)
        {
            var deleteResult = await _seatsCollection.DeleteOneAsync(s => s.SeatId == id);
            if (deleteResult.DeletedCount == 0)
            {
                return NotFound();
            }
            return NoContent();
        }
    }
}