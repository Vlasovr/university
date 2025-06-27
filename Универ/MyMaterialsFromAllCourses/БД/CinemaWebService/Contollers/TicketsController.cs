// using Microsoft.AspNetCore.Mvc;
// using Microsoft.EntityFrameworkCore;
// using CinemaWebService.Data;
// using CinemaWebService.Models;
//
// namespace CinemaWebService.Controllers
// {
//     [ApiController]
//     [Route("api/[controller]")]
//     public class TicketsController : ControllerBase
//     {
//         private readonly ApplicationDbContext _context;
//
//         public TicketsController(ApplicationDbContext context)
//         {
//             _context = context;
//         }
//
//         [HttpGet]
//         public async Task<ActionResult<IEnumerable<Ticket>>> GetTickets()
//         {
//             return await _context.Tickets
//                 // .Include(t => t.Client)
//                 // .Include(t => t.Session)
//                 // .Include(t => t.Seat)
//                 .ToListAsync();
//         }
//         
//         [HttpGet("{id}")]
//         public async Task<ActionResult<Ticket>> GetTicket(int id)
//         {
//             var ticket = await _context.Tickets
//                 // .Include(t => t.Client)
//                 // .Include(t => t.Session)
//                 // .Include(t => t.Seat)
//                 .FirstOrDefaultAsync(t => t.TicketId == id);
//
//             if (ticket == null)
//             {
//                 return NotFound();
//             }
//
//             return ticket;
//         }
//         
//         [HttpPost]
//         public async Task<ActionResult<Ticket>> PostTicket(Ticket ticket)
//         {
//             _context.Tickets.Add(ticket);
//             await _context.SaveChangesAsync();
//
//             return CreatedAtAction(nameof(GetTicket), new { id = ticket.TicketId }, ticket);
//         }
//         
//         [HttpPut("{id}")]
//         public async Task<IActionResult> PutTicket(int id, Ticket ticket)
//         {
//             if (id != ticket.TicketId)
//             {
//                 return BadRequest("Идентификаторы не совпадают");
//             }
//
//             _context.Entry(ticket).State = EntityState.Modified;
//
//             try
//             {
//                 await _context.SaveChangesAsync();
//             }
//             catch (DbUpdateConcurrencyException)
//             {
//                 if (!TicketExists(id))
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
//         public async Task<IActionResult> DeleteTicket(int id)
//         {
//             var ticket = await _context.Tickets.FindAsync(id);
//             if (ticket == null)
//             {
//                 return NotFound();
//             }
//
//             _context.Tickets.Remove(ticket);
//             await _context.SaveChangesAsync();
//
//             return NoContent();
//         }
//
//         private bool TicketExists(int id)
//         {
//             return _context.Tickets.Any(e => e.TicketId == id);
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
    public class TicketsController : ControllerBase
    {
        private readonly IMongoCollection<Ticket> _ticketsCollection;

        public TicketsController(IMongoDatabase database)
        {
            _ticketsCollection = database.GetCollection<Ticket>("tickets");
        }

        [HttpGet]
        public async Task<ActionResult<IEnumerable<Ticket>>> GetTickets()
        {
            var tickets = await _ticketsCollection.Find(_ => true).ToListAsync();
            return Ok(tickets);
        }

        [HttpGet("{id}")]
        public async Task<ActionResult<Ticket>> GetTicket(int id)
        {
            var ticket = await _ticketsCollection.Find(t => t.TicketId == id).FirstOrDefaultAsync();

            if (ticket == null)
            {
                return NotFound();
            }

            return Ok(ticket);
        }

        [HttpPost]
        public async Task<ActionResult<Ticket>> PostTicket(Ticket ticket)
        {
            // Генерация уникального TicketId (простой пример)
            var maxId = await _ticketsCollection.AsQueryable().MaxAsync(t => (int?)t.TicketId) ?? 0;
            ticket.TicketId = maxId + 1;
            ticket.PurchaseTime = DateTime.UtcNow;

            await _ticketsCollection.InsertOneAsync(ticket);

            return CreatedAtAction(nameof(GetTicket), new { id = ticket.TicketId }, ticket);
        }

        [HttpPut("{id}")]
        public async Task<IActionResult> PutTicket(int id, Ticket updatedTicket)
        {
            if (id != updatedTicket.TicketId)
            {
                return BadRequest("Идентификаторы не совпадают");
            }

            var replaceResult = await _ticketsCollection.ReplaceOneAsync(
                t => t.TicketId == id,
                updatedTicket
            );

            if (replaceResult.MatchedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }

        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteTicket(int id)
        {
            var deleteResult = await _ticketsCollection.DeleteOneAsync(t => t.TicketId == id);

            if (deleteResult.DeletedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }
    }
}