// using Microsoft.AspNetCore.Mvc;
// using Microsoft.EntityFrameworkCore;
// using CinemaWebService.Data;
// using CinemaWebService.Models;
//
// namespace CinemaWebService.Controllers
// {
//     [ApiController]
//     [Route("api/[controller]")]
//     public class SessionsController : ControllerBase
//     {
//         private readonly ApplicationDbContext _context;
//
//         public SessionsController(ApplicationDbContext context)
//         {
//             _context = context;
//         }
//
//         // GET: api/Sessions
//         [HttpGet]
//         public async Task<ActionResult<IEnumerable<Session>>> GetSessions()
//         {
//             return await _context.Sessions
//                 // .Include(s => s.SessionMovies)
//                 // .Include(s => s.SessionEmployees)
//                 // .Include(s => s.Hall)
//                 .ToListAsync();
//         }
//         
//         [HttpGet("{id}")]
//         public async Task<ActionResult<Session>> GetSession(int id)
//         {
//             var session = await _context.Sessions
//                 // .Include(s => s.SessionMovies)
//                 // .Include(s => s.SessionEmployees)
//                 // .Include(s => s.Hall)
//                 .FirstOrDefaultAsync(s => s.SessionId == id);
//
//             if (session == null)
//             {
//                 return NotFound();
//             }
//
//             return session;
//         }
//         
//         [HttpPost]
//         public async Task<ActionResult<Session>> PostSession(Session session)
//         {
//             _context.Sessions.Add(session);
//             await _context.SaveChangesAsync();
//
//             return CreatedAtAction(nameof(GetSession), new { id = session.SessionId }, session);
//         }
//         
//         [HttpPut("{id}")]
//         public async Task<IActionResult> PutSession(int id, Session session)
//         {
//             if (id != session.SessionId)
//             {
//                 return BadRequest("Идентификаторы не совпадают");
//             }
//
//             _context.Entry(session).State = EntityState.Modified;
//
//             try
//             {
//                 await _context.SaveChangesAsync();
//             }
//             catch (DbUpdateConcurrencyException)
//             {
//                 if (!SessionExists(id))
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
//         public async Task<IActionResult> DeleteSession(int id)
//         {
//             var session = await _context.Sessions.FindAsync(id);
//             if (session == null)
//             {
//                 return NotFound();
//             }
//
//             _context.Sessions.Remove(session);
//             await _context.SaveChangesAsync();
//
//             return NoContent();
//         }
//
//         private bool SessionExists(int id)
//         {
//             return _context.Sessions.Any(e => e.SessionId == id);
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
    public class SessionsController : ControllerBase
    {
        private readonly IMongoCollection<Session> _sessionsCollection;

        public SessionsController(IMongoDatabase database)
        {
            _sessionsCollection = database.GetCollection<Session>("sessions");
        }

        // GET: api/Sessions
        [HttpGet]
        public async Task<ActionResult<IEnumerable<Session>>> GetSessions()
        {
            var sessions = await _sessionsCollection.Find(_ => true).ToListAsync();
            return Ok(sessions);
        }
        
        [HttpGet("{id}")]
        public async Task<ActionResult<Session>> GetSession(int id)
        {
            var session = await _sessionsCollection.Find(s => s.SessionId == id).FirstOrDefaultAsync();
            if (session == null)
            {
                return NotFound();
            }

            return Ok(session);
        }
        
        [HttpPost]
        public async Task<ActionResult<Session>> PostSession(Session session)
        {
            // Простейший вариант автоинкремента (нужно смотреть макс. SessionId)
            // Или же используйте ObjectId
            var maxId = await _sessionsCollection.AsQueryable().MaxAsync(s => (int?)s.SessionId) ?? 0;
            session.SessionId = maxId + 1;

            await _sessionsCollection.InsertOneAsync(session);
            return CreatedAtAction(nameof(GetSession), new { id = session.SessionId }, session);
        }
        
        [HttpPut("{id}")]
        public async Task<IActionResult> PutSession(int id, Session session)
        {
            if (id != session.SessionId)
            {
                return BadRequest("Идентификаторы не совпадают");
            }

            var replaceResult = await _sessionsCollection.ReplaceOneAsync(s => s.SessionId == id, session);
            if (replaceResult.MatchedCount == 0)
            {
                return NotFound(); // Session не найдена
            }

            return NoContent();
        }
        
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteSession(int id)
        {
            var deleteResult = await _sessionsCollection.DeleteOneAsync(s => s.SessionId == id);
            if (deleteResult.DeletedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }
    }
}